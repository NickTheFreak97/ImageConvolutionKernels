#include "PPMImage.h"
#include <cassert>
#include <fstream>

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
PPMImage<IEEE754_t>::PPMImage(unsigned int width, unsigned int height, const Channel<IEEE754_t> *R, const Channel<IEEE754_t> *G, const Channel<IEEE754_t> *B) : Image<IEEE754_t>(
    width,
    height,
    [R, G, B] {
       auto channels = std::vector<Channel<IEEE754_t>*>();
        channels.push_back(R);
        channels.push_back(G);
        channels.push_back(B);
        return channels;
    }()
){  }


template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
PPMImage<IEEE754_t> *PPMImage<IEEE754_t>::filtered(const ConvolutionKernel<IEEE754_t> *usingKernel, const MatrixPaddingStrategy<IEEE754_t> *withPaddingStrategy) const {
    assert(this->getChannelCount() == 3);

    auto newChannels = std::vector<Channel<IEEE754_t> *>();

    for (auto channel : this->channels) {
        newChannels.push_back(channel->filtered(usingKernel, withPaddingStrategy));
    }

    return new PPMImage(this->width, this->height, newChannels[0], newChannels[1], newChannels[2]);
}


template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
void PPMImage<IEEE754_t>::writeChannelsToFile(const std::string& filename, const ImageChannelsEncoding& encoding) const {
    assert(this->getChannelCount() == 3);

    std::ofstream fileHandle;

    fileHandle.open((filename + ".ppm").c_str(), encoding == ImageChannelsEncoding::PLAIN ? std::ios::app : std::ios::app | std::ios::binary);

    if (fileHandle.fail()) {
        throw std::runtime_error("Could not open the specified file");
    }

    
    for (auto i = 0; i < this->getHeight(); i++) {
        for (auto j = 0; j < this->getWidth(); j++) {
            for (auto k = 0; k < this->getChannelCount(); k++) {
                auto currentChannel = this->getChannel(k);
                auto currentPixelValue = static_cast<int>(currentChannel->at(i, j));

                assert(currentPixelValue >= 0 && currentPixelValue <= 255);

                if (encoding == ImageChannelsEncoding::PLAIN) {
                    fileHandle << std::to_string(currentPixelValue);
                    if (i != this->getHeight() - 1 && j != this->getWidth() - 1) {
                        fileHandle << " ";
                    }
                } else {
                    auto pixelValueBinary = static_cast<std::byte>(currentPixelValue);
                    fileHandle.write(reinterpret_cast<const char*>(pixelValueBinary), 1);
                }
            }
        }
    }

    this->writeHeaderToFile(filename, encoding);
    this->writeChannelsToFile(filename, encoding);


    fileHandle.close();
}
