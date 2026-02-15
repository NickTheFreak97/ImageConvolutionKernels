#ifndef IMAGECONVOLUTIONKERNEL_NETPBMIMAGE_H
#define IMAGECONVOLUTIONKERNEL_NETPBMIMAGE_H

#include <optional>
#include <type_traits>
#include <filesystem>

#include "../Image.h"
#include "../ImageFormats/Header/NetpbmHeader.h"

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
class NetpbmImage: public Image<IEEE754_t> {
private:
    std::optional<NetpbmHeader*> header;

protected:
    std::optional<unsigned int> getExpectedChannelsCount();
    std::optional<unsigned int> getHeaderSpecifier(const ImageChannelsEncoding& forEncoding);
    std::optional<std::string> getFileExtension();
    std::optional<unsigned int> getMaxChannelValue();

public:
    NetpbmImage(unsigned int width, unsigned int height, std::initializer_list<Channel<IEEE754_t>*> channels);
    NetpbmImage(unsigned int width, unsigned int height, std::vector<Channel<IEEE754_t>*> channels);

    NetpbmImage* filtered(const ConvolutionKernel<IEEE754_t>* usingKernel, const MatrixPaddingStrategy<IEEE754_t>* withPaddingStrategy) const override;
    void writeHeaderToFile(const std::filesystem::path& filepath, const ImageChannelsEncoding& encoding) const override;
    void writeChannelsToFile(const std::filesystem::path& filepath, const ImageChannelsEncoding& encoding) const override;

    NetpbmImage* loadImage(const std::filesystem::path& filepath);
};


#endif