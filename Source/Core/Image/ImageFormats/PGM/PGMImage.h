#ifndef IMAGECONVOLUTIONKERNEL_PGMIMAGE_H
#define IMAGECONVOLUTIONKERNEL_PGMIMAGE_H

#include <type_traits>
#include <optional>
#include "../../Image.h"
#include "../Header/NetpmbHeader.h"

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
class PGMImage : public Image<IEEE754_t> {
private:
    std::optional<NetpbmHeader*> header;

public:
    PGMImage(unsigned int width, unsigned int height, Channel<IEEE754_t>* G, std::optional<NetpbmHeader*> header = std::nullopt);
    PGMImage* filtered(const ConvolutionKernel<IEEE754_t>* usingKernel, const MatrixPaddingStrategy<IEEE754_t>* withPaddingStrategy) const;

    void writeHeaderToFile(const std::filesystem::path& filepath, const ImageChannelsEncoding& encoding) const;
    void writeChannelsToFile(const std::filesystem::path& filepath, const ImageChannelsEncoding& encoding) const;
};


#endif