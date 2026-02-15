#include "NetpbmImage.h"

#include <cassert>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <string>
#include <utility>

#include "../../Utils/FileUtils.h"
#include "PPM/PPMImage.h"
#include "PGM//PGMImage.h"

template<typename IEEE754_t, typename Derived> requires std::is_floating_point_v<IEEE754_t>
NetpbmImage<IEEE754_t, Derived>::NetpbmImage(unsigned int width, unsigned int height, std::vector<Channel<IEEE754_t> *> channels, std::optional<NetpbmHeader*> header) : header(header), Image<IEEE754_t>(width, height, std::move(channels)) {

}


template<typename IEEE754_t, typename Derived> requires std::is_floating_point_v<IEEE754_t>
NetpbmImage<IEEE754_t, Derived>::NetpbmImage(unsigned int width, unsigned int height, std::initializer_list<Channel<IEEE754_t> *> channels) : Image<IEEE754_t>(width, height, channels) {

}

template<typename IEEE754_t, typename Derived> requires std::is_floating_point_v<IEEE754_t>
NetpbmImage<IEEE754_t, Derived> *NetpbmImage<IEEE754_t, Derived>::filtered(const ConvolutionKernel<IEEE754_t> *usingKernel, const MatrixPaddingStrategy<IEEE754_t> *withPaddingStrategy) const {
    assert(NetpbmImage::getExpectedChannelsCount().has_value());
    assert(this->getChannelsCount() == NetpbmImage::getExpectedChannelsCount());

    auto newChannels = std::vector<Channel<IEEE754_t> *>();

    for (int i = 0; i < this->getChannelsCount(); i++) {
        auto channel = this->getChannel(i);
        newChannels.push_back(channel->filtered(usingKernel, withPaddingStrategy));
    }

    return new Derived(NetpbmImage::getWidth(), NetpbmImage::getHeight(), newChannels);
}

template<typename IEEE754_t, typename Derived> requires std::is_floating_point_v<IEEE754_t>
void NetpbmImage<IEEE754_t, Derived>::writeHeaderToFile(const std::filesystem::path& filepath, const ImageChannelsEncoding& encoding) const {
    assert(this->getExpectedChannelsCount().has_value());
    assert(this->getHeaderSpecifier(encoding).has_value() || this->header.has_value());
    assert(this->getFileExtension().has_value());
    assert(this->getMaxChannelValue().has_value() || this->header.has_value());
    assert(this->getChannelsCount() == NetpbmImage::getExpectedChannelsCount());

    std::ofstream fileHandle;

    fileHandle.open((filepath.string() + "." + NetpbmImage::getFileExtension().value()).c_str(), encoding == ImageChannelsEncoding::PLAIN ? std::ios::trunc : std::ios::trunc | std::ios::binary);

    if (fileHandle.fail()) {
        throw std::runtime_error("Could not open the specified file");
    }

    if (this->header.has_value()) {
        fileHandle << "P" << std::to_string(static_cast<int>(this->header.value()->getFormat())) << std::endl;
        fileHandle << std::to_string(this->header.value()->getColumns()) << " " << std::to_string(this->header.value()->getRows()) << std::endl;
        fileHandle << std::to_string(this->header.value()->getMaxPixelValue()) << std::endl;
    } else {
        fileHandle << "P" << (NetpbmImage::getHeaderSpecifier(encoding).value()) << std::endl;
        fileHandle << std::to_string(this->getWidth()) << " " << std::to_string(this->getHeight()) << std::endl;
        fileHandle << std::to_string(NetpbmImage::getMaxChannelValue().value()) << std::endl;
    }

    fileHandle.close();
}


template<typename IEEE754_t, typename Derived> requires std::is_floating_point_v<IEEE754_t>
void NetpbmImage<IEEE754_t, Derived>::writeChannelsToFile(const std::filesystem::path& filepath, const ImageChannelsEncoding& encoding) const {
    assert(this->getExpectedChannelsCount().has_value());
    assert(this->getHeaderSpecifier(encoding).has_value() || this->header.has_value());
    assert(this->getFileExtension().has_value());
    assert(this->getMaxChannelValue().has_value() || this->header.has_value());
    assert(this->getChannelsCount() == NetpbmImage::getExpectedChannelsCount());

    std::ofstream fileHandle;

    fileHandle.open((filepath.string() + "." + NetpbmImage::getFileExtension().value()).c_str(),
        encoding == ImageChannelsEncoding::PLAIN ? std::ios::app : std::ios::app | std::ios::binary);

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

                if (this->header.has_value()) {
                    assert(currentPixelValue >= 0 && currentPixelValue <= this->header.value()->getMaxPixelValue());
                } else {
                    assert(currentPixelValue >= 0 && currentPixelValue <= NetpbmImage::getMaxChannelValue().value());
                }

                if (encoding == ImageChannelsEncoding::PLAIN) {
                    fileHandle << std::to_string(currentPixelValue) << " ";
                } else {
                    if (NetpbmImage::getMaxChannelValue().value() < 256) {
                        auto byte = static_cast<uint8_t>(currentPixelValue);
                        fileHandle.write(reinterpret_cast<const char*>(&byte), 1);
                    } else {
                        auto word = static_cast<uint16_t>(currentPixelValue);
                        uint8_t bytes[2] = {static_cast<uint8_t>(word >> 8), static_cast<uint8_t>(word & 0xFF)};
                        fileHandle.write(reinterpret_cast<const char*>(bytes), 2);
                    }
                }
            }
        }
    }

    fileHandle.close();
}





template<typename IEEE754_t, typename Derived> requires std::is_floating_point_v<IEEE754_t>
NetpbmImage<IEEE754_t, Derived> *NetpbmImage<IEEE754_t, Derived>::loadImage(const std::filesystem::path &filepath) {
    assert(NetpbmImage::getExpectedChannelsCount().has_value());
    auto parsedHeader = NetpbmHeader::parsing(filepath);

    if (!parsedHeader) {
        return nullptr;
    }

    std::ifstream fileHandle(filepath.string(), std::ios::binary);
    fileHandle.seekg(parsedHeader->getPositionOfFirstPixel());

    auto channels = std::vector<std::vector<IEEE754_t>>();
    for (auto i = 0; i < NetpbmImage::getExpectedChannelsCount(); i++) {
        channels.push_back(std::vector<IEEE754_t>());
    }

    auto inputPixelValueCount = 0;
    auto pixelsPerValue = static_cast<unsigned int>(ceil(log2(static_cast<float>(parsedHeader->getMaxPixelValue() + 1)) / 8));

    while (true) {
        std::optional<std::string> nextValue;

        if (parsedHeader->getFormat() == ImageNetpbmFormat::PPM_BINARY) {
            nextValue = FileUtils::getNextBytes(fileHandle, pixelsPerValue);
        } else {
            nextValue = FileUtils::getNextWord(fileHandle);
        }

        if (nextValue.has_value()) {
            auto pixelValue = static_cast<IEEE754_t>(std::stol(nextValue.value()));
            assert(pixelValue >= 0 && pixelValue <= parsedHeader->getMaxPixelValue());

            channels[inputPixelValueCount % NetpbmImage::getExpectedChannelsCount().value()].push_back(pixelValue);
            inputPixelValueCount++;
        } else {
            break;
        }
    }

    assert(inputPixelValueCount == parsedHeader->getRows() * parsedHeader->getColumns() * NetpbmImage::getExpectedChannelsCount().value());

    fileHandle.close();

    auto outputChannels = std::vector<Channel<IEEE754_t>*>();
    std::ranges::transform(channels, std::back_inserter(outputChannels),
    [parsedHeader](std::vector<IEEE754_t>& channelValues) {  // Pass by REFERENCE
        return new Channel<IEEE754_t>(
            parsedHeader->getMaxPixelValue(),
            channelValues.data(),
            parsedHeader->getRows(),
            parsedHeader->getColumns()
        );
    });

    return new Derived(
        parsedHeader->getColumns(),
        parsedHeader->getRows(),
        outputChannels,
        std::optional(parsedHeader)
    );
}


template<typename IEEE754_t, typename Derived> requires std::is_floating_point_v<IEEE754_t>
std::optional<unsigned int> NetpbmImage<IEEE754_t, Derived>::getExpectedChannelsCount() {
    return Derived::getExpectedChannelsCount();
}

template<typename IEEE754_t, typename Derived> requires std::is_floating_point_v<IEEE754_t>
std::optional<unsigned int> NetpbmImage<IEEE754_t, Derived>::getHeaderSpecifier(const ImageChannelsEncoding& forEncoding) {
    return Derived::getHeaderSpecifier(forEncoding);
}

template<typename IEEE754_t, typename Derived> requires std::is_floating_point_v<IEEE754_t>
std::optional<std::string> NetpbmImage<IEEE754_t, Derived>::getFileExtension() {
    return Derived::getFileExtension();
}

template<typename IEEE754_t, typename Derived> requires std::is_floating_point_v<IEEE754_t>
std::optional<unsigned int> NetpbmImage<IEEE754_t, Derived>::getMaxChannelValue() {
    return Derived::getMaxChannelValue();
}


template class NetpbmImage<float, PPMImage<float>>;
template class NetpbmImage<double, PPMImage<double>>;
template class NetpbmImage<long double, PPMImage<long double>>;

template class NetpbmImage<float, PGMImage<float>>;
template class NetpbmImage<double, PGMImage<double>>;
template class NetpbmImage<long double, PGMImage<long double>>;
