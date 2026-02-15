#ifndef IMAGECONVOLUTIONKERNEL_PGMIMAGE_H
#define IMAGECONVOLUTIONKERNEL_PGMIMAGE_H

#include <type_traits>
#include <optional>
#include "../../Image.h"
#include "../Header/NetpbmHeader.h"
#include "../../ImageFormats/NetpbmImage.h"

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
class PGMImage : public NetpbmImage<IEEE754_t, PGMImage<IEEE754_t>> {
    friend class NetpbmImage<IEEE754_t, PGMImage>;
private:
    std::optional<NetpbmHeader*> header;

public:
    PGMImage(unsigned int width, unsigned int height, Channel<IEEE754_t>* G, std::optional<NetpbmHeader*> header = std::nullopt);
    PGMImage* filtered(const ConvolutionKernel<IEEE754_t>* usingKernel, const MatrixPaddingStrategy<IEEE754_t>* withPaddingStrategy) const override;

protected:
    PGMImage(unsigned int width, unsigned int height, std::vector<Channel<IEEE754_t>*> channels, std::optional<NetpbmHeader*> header = std::nullopt);
    [[nodiscard]] static std::optional<unsigned int> getExpectedChannelsCount();
    [[nodiscard]] static std::optional<unsigned int> getHeaderSpecifier(const ImageChannelsEncoding& forEncoding);
    [[nodiscard]] static std::optional<std::string> getFileExtension();
    [[nodiscard]] static std::optional<unsigned int> getMaxChannelValue();
};


#endif