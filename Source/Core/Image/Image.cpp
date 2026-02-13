#include "Image.h"
#include <cassert>

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
Image<IEEE754_t>::Image(unsigned int width, unsigned int height, std::initializer_list<Channel<IEEE754_t> *> channels) : width(width), height(height) {
    for (auto channel : channels) {
        assert(channel->getRows() == height);
        assert(channel->getColumns() == width);

        this->channels.push_back(channel);
    }
}

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
Image<IEEE754_t>::Image(unsigned int width, unsigned int height, std::vector<Channel<IEEE754_t> *> channels) : width(width), height(height) {
    for (auto channel : channels) {
        assert(channel->getRows() == height);
        assert(channel->getColumns() == width);

        this->channels.push_back(channel);
    }
}

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
Image<IEEE754_t>::~Image() {
    this->channels.clear();
}

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
unsigned int Image<IEEE754_t>::getWidth() const {
    return this->width;
}

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
unsigned int Image<IEEE754_t>::getHeight() const {
    return this->height;
}

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
unsigned int Image<IEEE754_t>::getChannelCount() const {
    return this->channels.size();
}

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
Channel<IEEE754_t>* Image<IEEE754_t>::getChannel(unsigned int channelIndex) const {
    return this->channels.at(channelIndex);
}

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
Image<IEEE754_t> *Image<IEEE754_t>::filtered(const ConvolutionKernel<IEEE754_t> *usingKernel, const MatrixPaddingStrategy<IEEE754_t> *withPaddingStrategy) const {
    auto newChannels = std::vector<Channel<IEEE754_t> *>();

    for (auto channel : this->channels) {
        newChannels.push_back(channel->filtered(usingKernel, withPaddingStrategy));
    }

    return new Image(this->width, this->height, newChannels);
}

template class Image<float>;
template class Image<double>;
template class Image<long double>;
