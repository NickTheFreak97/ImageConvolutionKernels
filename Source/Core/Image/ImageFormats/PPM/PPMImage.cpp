#include "../PPM/PPMImage.h"
#include <cmath>
#include <cassert>
#include <vector>

#include "../../../Utils/FileUtils.h"

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
PPMImage<IEEE754_t>::PPMImage(unsigned int width, unsigned int height, Channel<IEEE754_t> *R, Channel<IEEE754_t> *G, Channel<IEEE754_t> *B, std::optional<NetpbmHeader*> header) : NetpbmImage<IEEE754_t, PPMImage>(
    width,
    height,
    [R, G, B] {
       auto channels = std::vector<Channel<IEEE754_t>*>();
        channels.push_back(R);
        channels.push_back(G);
        channels.push_back(B);
        return channels;
    }(),
    header
){  }

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
PPMImage<IEEE754_t>::PPMImage(unsigned int width, unsigned int height, std::vector<Channel<IEEE754_t>*> channels, std::optional<NetpbmHeader*> header)
    : NetpbmImage<IEEE754_t, PPMImage>(width, height, channels, header) {
    assert(channels.size() == 3 && "PPMImage must have exactly 3 channels (R, G, B)");
}



template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
[[nodiscard]] std::optional<unsigned int> PPMImage<IEEE754_t>::getExpectedChannelsCount() {
    return std::optional<unsigned int>(3);
}

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
[[nodiscard]] std::optional<unsigned int> PPMImage<IEEE754_t>::getHeaderSpecifier(const ImageChannelsEncoding& forEncoding) {
    switch (forEncoding) {
        case ImageChannelsEncoding::PLAIN:
            return 3;
        case ImageChannelsEncoding::BINARY:
            return 6;
        default:
            return std::nullopt;
    }
}



template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
PPMImage<IEEE754_t> *PPMImage<IEEE754_t>::filtered(const ConvolutionKernel<IEEE754_t> *usingKernel, const MatrixPaddingStrategy<IEEE754_t> *withPaddingStrategy) const {
    assert(PPMImage::getExpectedChannelsCount().has_value());
    assert(this->getChannelsCount() == PPMImage::getExpectedChannelsCount());

    auto newChannels = std::vector<Channel<IEEE754_t> *>();

    for (int i = 0; i < this->getChannelsCount(); i++) {
        auto channel = this->getChannel(i);
        newChannels.push_back(channel->filtered(usingKernel, withPaddingStrategy));
    }

    return new PPMImage(PPMImage::getWidth(), PPMImage::getHeight(), newChannels);

}


template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
[[nodiscard]] std::optional<std::string> PPMImage<IEEE754_t>::getFileExtension() {
    return std::optional<std::string>("ppm");
}

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
[[nodiscard]] std::optional<unsigned int> PPMImage<IEEE754_t>::getMaxChannelValue() {
    return std::optional<unsigned int>(255);
}


template class PPMImage<float>;
template class PPMImage<double>;
template class PPMImage<long double>;