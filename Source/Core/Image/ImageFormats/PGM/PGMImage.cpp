#include "PGMImage.h"
#include <cassert>
#include <fstream>
#include "../../Image.h"

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
PGMImage<IEEE754_t>::PGMImage(unsigned int width, unsigned int height, Channel<IEEE754_t> *G, std::optional<NetpbmHeader *> header) : header(header), NetpbmImage<IEEE754_t, PGMImage>(width, height, {G}) {

}

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
PGMImage<IEEE754_t>::PGMImage(unsigned int width, unsigned int height, std::vector<Channel<IEEE754_t>*> channels, std::optional<NetpbmHeader*> header)
    : NetpbmImage<IEEE754_t, PGMImage>(width, height, channels, header) {
    assert(channels.size() == 1 && "PPMImage must have exactly 1 channel: G");
}


template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
[[nodiscard]] std::optional<unsigned int> PGMImage<IEEE754_t>::getExpectedChannelsCount() {
    return std::optional<unsigned int>(1);
}

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
[[nodiscard]] std::optional<unsigned int> PGMImage<IEEE754_t>::getHeaderSpecifier(const ImageChannelsEncoding& forEncoding) {
    switch (forEncoding) {
        case ImageChannelsEncoding::PLAIN:
            return 2;
        case ImageChannelsEncoding::BINARY:
            return 5;
        default:
            return std::nullopt;
    }
}



template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
PGMImage<IEEE754_t> *PGMImage<IEEE754_t>::filtered(const ConvolutionKernel<IEEE754_t> *usingKernel, const MatrixPaddingStrategy<IEEE754_t> *withPaddingStrategy) const {
    assert(PGMImage::getExpectedChannelsCount().has_value());
    assert(this->getChannelsCount() == PGMImage::getExpectedChannelsCount());

    auto newChannels = std::vector<Channel<IEEE754_t> *>();

    for (int i = 0; i < this->getChannelsCount(); i++) {
        auto channel = this->getChannel(i);
        newChannels.push_back(channel->filtered(usingKernel, withPaddingStrategy));
    }

    return new PGMImage(this->getWidth(), this->getHeight(), newChannels);
}


template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
[[nodiscard]] std::optional<std::string> PGMImage<IEEE754_t>::getFileExtension() {
    return std::optional<std::string>("pgm");
}

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
[[nodiscard]] std::optional<unsigned int> PGMImage<IEEE754_t>::getMaxChannelValue() {
    return std::optional<unsigned int>(65535);
}


template class PGMImage<float>;
template class PGMImage<double>;
template class PGMImage<long double>;
