
#ifndef IMAGECONVOLUTIONKERNEL_PPMIMAGE_H
#define IMAGECONVOLUTIONKERNEL_PPMIMAGE_H

#include <type_traits>
#include <filesystem>

#include "../Image.h"
#include "Header/NetpmbHeader.h"

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
class PPMImage : public Image<IEEE754_t> {
private:
    std::optional<NetpmbHeader*> header;
public:
    PPMImage(unsigned int width, unsigned int height, Channel<IEEE754_t>* R, Channel<IEEE754_t>* G, Channel<IEEE754_t>* B, std::optional<NetpmbHeader*> header = std::nullopt);

    PPMImage* filtered(const ConvolutionKernel<IEEE754_t>* usingKernel, const MatrixPaddingStrategy<IEEE754_t>* withPaddingStrategy) const;
    void writeHeaderToFile(const std::filesystem::path& filepath, const ImageChannelsEncoding& encoding) const;
    void writeChannelsToFile(const std::filesystem::path& filepath, const ImageChannelsEncoding& encoding) const;

    static PPMImage* loadImage(const std::filesystem::path& filepath);
};


#endif