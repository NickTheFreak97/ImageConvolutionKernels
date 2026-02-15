#include "../PPM/PPMImage.h"
#include <cassert>
#include <cmath>
#include <fstream>
#include <vector>

#include "../../../Utils/FileUtils.h"

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
PPMImage<IEEE754_t>::PPMImage(unsigned int width, unsigned int height, Channel<IEEE754_t> *R, Channel<IEEE754_t> *G, Channel<IEEE754_t> *B, std::optional<NetpbmHeader*> header) : header(header), Image<IEEE754_t>(
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
    assert(this->getChannelsCount() == 3);

    auto newChannels = std::vector<Channel<IEEE754_t> *>();

    for (int i = 0; i < this->getChannelsCount(); i++) {
        auto channel = this->getChannel(i);
        newChannels.push_back(channel->filtered(usingKernel, withPaddingStrategy));
    }

    return new PPMImage(this->getWidth(), this->getHeight(), newChannels[0], newChannels[1], newChannels[2]);
}


template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
void PPMImage<IEEE754_t>::writeHeaderToFile(const std::filesystem::path& filepath, const ImageChannelsEncoding& encoding) const {
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
void PPMImage<IEEE754_t>::writeChannelsToFile(const std::filesystem::path& filepath, const ImageChannelsEncoding& encoding) const {
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


template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
PPMImage<IEEE754_t>* PPMImage<IEEE754_t>::loadImage(const std::filesystem::path &filepath) {
    auto header = NetpbmHeader::parsing(filepath);

    if (!header) {
        return nullptr;
    }

    std::ifstream fileHandle(filepath.string(), std::ios::binary);
    fileHandle.seekg(header->getPositionOfFirstPixel());

    auto red = std::vector<IEEE754_t>();
    auto green = std::vector<IEEE754_t>();
    auto blue = std::vector<IEEE754_t>();

    auto inputPixelValueCount = 0;
    auto pixelsPerValue = static_cast<unsigned int>(ceil(log2(static_cast<float>(header->getMaxPixelValue() + 1)) / 8));

    while (true) {
        std::optional<std::string> nextValue;

        if (header->getFormat() == ImageNetpbmFormat::PPM_BINARY) {
            nextValue = FileUtils::getNextBytes(fileHandle, pixelsPerValue);
        } else {
            nextValue = FileUtils::getNextWord(fileHandle);
        }

        if (nextValue.has_value()) {
            auto pixelValue = static_cast<IEEE754_t>(std::stol(nextValue.value()));
            assert(pixelValue >= 0 && pixelValue <= header->getMaxPixelValue());

            switch (inputPixelValueCount % 3) {
                case 0:
                    red.push_back(pixelValue);
                    break;
                case 1:
                    green.push_back(pixelValue);
                    break;
                case 2:
                    blue.push_back(pixelValue);
                    break;

                default:
                    break;
            }

            inputPixelValueCount++;
        } else {
            break;
        }
    }

    assert(inputPixelValueCount == header->getRows() * header->getColumns() * 3);

    fileHandle.close();

    auto redChannel = new Channel<IEEE754_t>(
        header->getMaxPixelValue(),
        red.data(),
        header->getRows(),
        header->getColumns()
    );

    auto greenChannel = new Channel<IEEE754_t>(
        header->getMaxPixelValue(),
        green.data(),
        header->getRows(),
        header->getColumns()
    );

    auto blueChannel = new Channel<IEEE754_t>(
        header->getMaxPixelValue(),
        blue.data(),
        header->getRows(),
        header->getColumns()
    );

    return new PPMImage(
        header->getColumns(),
        header->getRows(),
        redChannel,
        greenChannel,
        blueChannel,
        std::optional(header)
    );
}


template class PPMImage<float>;
template class PPMImage<double>;
template class PPMImage<long double>;