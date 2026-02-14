#ifndef IMAGECONVOLUTIONKERNEL_IMAGE_H
#define IMAGECONVOLUTIONKERNEL_IMAGE_H

#include <type_traits>
#include <vector>
#include <filesystem>
#include <initializer_list>

#include "../Channel/Channel.h"

enum class ImageChannelsEncoding {
    PLAIN = 0,
    BINARY = 1,
};

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
class Image {
private:
    unsigned int width;
    unsigned int height;
    std::vector<Channel<IEEE754_t>*> channels;

public:
    Image(unsigned int width, unsigned int height, std::initializer_list<Channel<IEEE754_t>*> channels);
    Image(unsigned int width, unsigned int height, std::vector<Channel<IEEE754_t>*> channels);
    ~Image();

    virtual Image* filtered(const ConvolutionKernel<IEEE754_t>* usingKernel, const MatrixPaddingStrategy<IEEE754_t>* withPaddingStrategy) const = 0;

    [[nodiscard]] unsigned int getWidth() const;
    [[nodiscard]] unsigned int getHeight() const;
    [[nodiscard]] unsigned int getChannelsCount() const;
    Channel<IEEE754_t>* getChannel(unsigned int channelIndex) const;

    void writeToFile(const std::string& filename, const ImageChannelsEncoding& encoding) const;
    virtual void writeHeaderToFile(const std::filesystem::path& filepath, const ImageChannelsEncoding& encoding) const = 0;
    virtual void writeChannelsToFile(const std::filesystem::path& filepath, const ImageChannelsEncoding& encoding) const = 0;
};

#endif


