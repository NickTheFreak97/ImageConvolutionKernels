#include <type_traits>
#include "../ConvolutionKernel.h"

namespace Kernels {
    template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
    ConvolutionKernel<IEEE754_t> *averageKernel(unsigned int size) {
        auto elements = new IEEE754_t[size * size];

        for (int i = 0; i < size * size; i++) {
            elements[i] = 1.0/(size * size);
        }

        return new ConvolutionKernel<IEEE754_t>(elements, size, size, ROW_MAJOR);
    }
}