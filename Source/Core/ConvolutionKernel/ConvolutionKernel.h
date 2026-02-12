
#ifndef IMAGECONVOLUTIONKERNEL_CONVOLUTIONKERNEL_H
#define IMAGECONVOLUTIONKERNEL_CONVOLUTIONKERNEL_H

#include <type_traits>
#include "../Matrix/Matrix.h"

template <typename IEEE754_t>
    requires std::is_floating_point_v<IEEE754_t>
class ConvolutionKernel : public Matrix<IEEE754_t> {
    public:
    ConvolutionKernel(const IEEE754_t* elements, unsigned int rows, unsigned int columns, MatrixLayout layout = ROW_MAJOR);
    explicit ConvolutionKernel(const Matrix<IEEE754_t>* fromMatrix);


    int getCentralRowIndex();
    int getCentralColumnIndex();

    int getLowerBoundRowIndex();
    int getUpperBoundRowIndex();

    int getLowerBoundColumnIndex();
    int getUpperBoundColumnIndex();
};

extern template class Matrix<float>;
extern template class Matrix<double>;
extern template class Matrix<long double>;

#endif