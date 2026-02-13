#include <type_traits>
#include <cmath>
#include <limits>

#include "../ConvolutionKernel.h"
/*
* G(x, y) = (1 / (2πσ²)) * exp(-(x² + y²) / (2σ²))
 * - x is the distance of the row index from the center of the kernel
 * - y is the distance of the column index from the center of the kernel
 * - Returns: A normalized kernel generated according to the above formula.
 *
 * - Note: A common choice is that σ is three times the width of the kernel in each direction, i.e. `size = 6*sigma`.
 * Another choice is `sigma = 0.3 * ((size - 1) * 0.5 - 1) + 0.8`, as used by OpenCV. Also for numerical stability sigma should be at least 0.5.
 */
namespace Kernels {
    template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
    ConvolutionKernel<IEEE754_t> *gaussianKernel(unsigned int size, IEEE754_t sigma) {
        auto elements = new IEEE754_t[size * size];
        IEEE754_t sumOfValues = 0;

        if (size % 2 != 0) {
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    auto x = (static_cast<IEEE754_t>(i) - static_cast<IEEE754_t>(size - 1) / 2);
                    auto y = (static_cast<IEEE754_t>(j) - static_cast<IEEE754_t>(size - 1) / 2);

                    elements[i * size + j] = 1.0 / (2 * M_PI * sigma * sigma) * exp(-(x*x + y*y) / (2*sigma * sigma));
                    sumOfValues += elements[i * size + j];
                }
            }
        } else {
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    auto x = (static_cast<IEEE754_t>(i) - static_cast<IEEE754_t>(size - 1) / 2);
                    auto y = (static_cast<IEEE754_t>(j) - static_cast<IEEE754_t>(size - 1) / 2);

                    x += (x < 0) ? -0.5 : 0.5;
                    y += (y < 0) ? -0.5 : 0.5;

                    elements[i * size + j] = 1.0 / (2 * M_PI * sigma * sigma) * exp(-(x*x + y*y) / (2*sigma * sigma));
                    sumOfValues += elements[i * size + j];
                }
            }
        }


        for (int i = 0; i < size * size; i++) {
            elements[i] /= sumOfValues;
        }

        return new ConvolutionKernel(elements, size, size, ROW_MAJOR);
    }
}