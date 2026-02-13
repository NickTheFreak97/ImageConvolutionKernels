#include <type_traits>
#include <cassert>
#include  "../ConvolutionKernel.h"

namespace Kernels {
    template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
    ConvolutionKernel<IEEE754_t>* identity(unsigned int size) {
        assert(size % 2 != 0);

        auto elements = new IEEE754_t[size * size];
        memset(elements, 0, size * sizeof(IEEE754_t));

        elements[(size - 1)/2 * size + (size - 1)/2] = 1.0;

        return new ConvolutionKernel<IEEE754_t>(
            elements,
            size,
            size,
            ROW_MAJOR
        );
    }
}