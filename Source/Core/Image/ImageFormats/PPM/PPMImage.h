
#ifndef IMAGECONVOLUTIONKERNEL_PPMIMAGE_H
#define IMAGECONVOLUTIONKERNEL_PPMIMAGE_H

#include <type_traits>
#include <filesystem>

#include "../../Image.h"
#include "../Header/NetpbmHeader.h"
#include "../NetpbmImage.h"

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
class PPMImage : public NetpbmImage<IEEE754_t, PPMImage<IEEE754_t>> {
    friend class NetpbmImage<IEEE754_t, PPMImage>;
protected:
    [[nodiscard]] static std::optional<unsigned int> getExpectedChannelsCount();
    [[nodiscard]] static std::optional<unsigned int> getHeaderSpecifier(const ImageChannelsEncoding& forEncoding);
    [[nodiscard]] static std::optional<std::string> getFileExtension();
    [[nodiscard]] static std::optional<unsigned int> getMaxChannelValue();

public:
    PPMImage(unsigned int width, unsigned int height, Channel<IEEE754_t>* R, Channel<IEEE754_t>* G, Channel<IEEE754_t>* B, std::optional<NetpbmHeader*> header = std::nullopt);
    PPMImage* filtered(const ConvolutionKernel<IEEE754_t>* usingKernel, const MatrixPaddingStrategy<IEEE754_t>* withPaddingStrategy) const override;

protected:
    PPMImage(unsigned int width, unsigned int height, std::vector<Channel<IEEE754_t>*> channels, std::optional<NetpbmHeader*> header = std::nullopt);
};


#endif