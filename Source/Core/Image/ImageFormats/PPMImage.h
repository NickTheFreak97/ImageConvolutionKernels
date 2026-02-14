
#ifndef IMAGECONVOLUTIONKERNEL_PPMIMAGE_H
#define IMAGECONVOLUTIONKERNEL_PPMIMAGE_H

#include <type_traits>

#include "../Image.h"

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
class PPMImage : public Image<IEEE754_t> {
public:
    PPMImage(unsigned int width, unsigned int height, const Channel<IEEE754_t>* R, const Channel<IEEE754_t>* G, const Channel<IEEE754_t>* B);

    PPMImage* filtered(const ConvolutionKernel<IEEE754_t>* usingKernel, const MatrixPaddingStrategy<IEEE754_t>* withPaddingStrategy) const;
    void writeHeaderToFile(const std::string& filename, const ImageChannelsEncoding& encoding) const;
    void writeChannelsToFile(const std::string& filename, const ImageChannelsEncoding& encoding) const;

};


#endif