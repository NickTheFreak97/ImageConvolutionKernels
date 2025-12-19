#ifndef IMAGECONVOLUTIONKERNEL_MATRIXPADDINGSTRATEGY_H
#define IMAGECONVOLUTIONKERNEL_MATRIXPADDINGSTRATEGY_H
#include <type_traits>
#include "../Matrix/Matrix.h"

template<typename  IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
class MatrixPaddingStrategy {
public:
    /*
     * This method defines the values of the `matrix` parameter for indices that are out of its natural bounds.
     *
     * @Parameter matrix: The input matrix to pad.
     * @Parameter row: The row index to access.
     * @Parameter column: The column index to access.
     *
     * If bot `row` and `column` parameter are within natural bounds of the matrix, then the output of this function should be the value of the matrix at [row][column].
     * If at least one between the two `unsigned integer` parameters are not within the natural bounds of the matrix, a new value that may depend on the input matrix is returned, depending on the specific implementation.
     *
     * The purpose of this method is to parametrize what happens when you apply a convolution kernel at an output pixel where at least part of the kernel falls out of bounds with respect to the input matrix.
     */
    virtual IEEE754_t pad(const Matrix<IEEE754_t>&, int,  int) const = 0;
    virtual ~MatrixPaddingStrategy() = default;
};


#endif