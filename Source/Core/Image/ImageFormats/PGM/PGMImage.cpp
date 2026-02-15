#include "PGMImage.h"
#include <cassert>
#include <fstream>
#include "../../Image.h"

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
PGMImage<IEEE754_t>::PGMImage(unsigned int width, unsigned int height, Channel<IEEE754_t> *G, std::optional<NetpbmHeader *> header) : header(header), Image<IEEE754_t>(width, height, {G}) {

}

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
PGMImage<IEEE754_t> *PGMImage<IEEE754_t>::filtered(const ConvolutionKernel<IEEE754_t> *usingKernel, const MatrixPaddingStrategy<IEEE754_t> *withPaddingStrategy) const {
    assert(this->getChannelsCount() == 1);

    auto newChannels = std::vector<Channel<IEEE754_t> *>();

    for (int i = 0; i < this->getChannelsCount(); i++) {
        auto channel = this->getChannel(i);
        newChannels.push_back(channel->filtered(usingKernel, withPaddingStrategy));
    }

    return new PGMImage(this->getWidth(), this->getHeight(), newChannels[0], header);
}


template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
void PGMImage<IEEE754_t>::writeHeaderToFile(const std::filesystem::path& filepath, const ImageChannelsEncoding& encoding) const {
    assert(this->getChannelsCount() == 3);

    std::ofstream fileHandle;

    fileHandle.open((filepath.string() + ".ppm").c_str(), encoding == ImageChannelsEncoding::PLAIN ? std::ios::trunc : std::ios::trunc | std::ios::binary);

    if (fileHandle.fail()) {
        throw std::runtime_error("Could not open the specified file");
    }

    fileHandle << "P" << ((encoding == ImageChannelsEncoding::PLAIN) ? "3" : "6") << std::endl;
    fileHandle << std::to_string(this->getWidth()) << " " << std::to_string(this->getHeight()) << std::endl;
    fileHandle << "255" << std::endl;

    fileHandle.close();
}

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
void PGMImage<IEEE754_t>::writeChannelsToFile(const std::filesystem::path& filepath, const ImageChannelsEncoding& encoding) const {
    assert(this->getChannelsCount() == 3);

    std::ofstream fileHandle;

    fileHandle.open((filepath.string() + ".ppm").c_str(), encoding == ImageChannelsEncoding::PLAIN ? std::ios::app : std::ios::app | std::ios::binary);

    if (fileHandle.fail()) {
        throw std::runtime_error("Could not open the specified file");
    }

    for (auto i = 0; i < this->getHeight(); i++) {
        for (auto j = 0; j < this->getWidth(); j++) {
            for (auto k = 0; k < this->getChannelsCount(); k++) {

                auto currentChannel = this->getChannel(k);
                if (currentChannel->getMatrixLayout() == COLUMN_MAJOR) {
                    currentChannel = currentChannel->transposedChannel();
                }

                int currentPixelValue = static_cast<int>(currentChannel->at(i, j));

                assert(currentPixelValue >= 0 && currentPixelValue <= 255);

                if (encoding == ImageChannelsEncoding::PLAIN) {
                    fileHandle << std::to_string(currentPixelValue) << " ";
                } else {
                    fileHandle.write(reinterpret_cast<const char*>(&currentPixelValue), 1);
                }
            }
        }
    }

    fileHandle.close();
}

