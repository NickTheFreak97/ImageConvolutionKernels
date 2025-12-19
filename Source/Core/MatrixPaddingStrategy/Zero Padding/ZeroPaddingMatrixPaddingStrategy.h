#ifndef IMAGECONVOLUTIONKERNEL_ZEROPADDINGMATRIXPADDINGSTRATEGY_H
#define IMAGECONVOLUTIONKERNEL_ZEROPADDINGMATRIXPADDINGSTRATEGY_H

#include <type_traits>
#include "../MatrixPaddingStrategy.h"

template <typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
class ZeroPaddingMatrixPaddingStrategy : public MatrixPaddingStrategy<IEEE754_t> {
public:
     IEEE754_t pad(const Matrix<IEEE754_t>&, int, int) const override;
};


#endif