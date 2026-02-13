#ifndef IMAGECONVOLUTIONKERNEL_CHANNEL_H
#define IMAGECONVOLUTIONKERNEL_CHANNEL_H

#include <type_traits>
#include <gtest/gtest_prod.h>

#include "../ConvolutionKernel/ConvolutionKernel.h"
#include "../Matrix/Matrix.h"
#include "../MatrixPaddingStrategy/MatrixPaddingStrategy.h"

template<typename IEEE754_t>
    requires std::is_floating_point_v<IEEE754_t>
class Channel: public Matrix<IEEE754_t> {

private:
    unsigned int maxTheoreticalValue;
    bool isWithinMaxThreshold();
    IEEE754_t outputPixel(
        unsigned int row,
        unsigned int column,
        const ConvolutionKernel<IEEE754_t>* usingKernel,
        const MatrixPaddingStrategy<IEEE754_t>* paddingStrategy
    ) const;

    FRIEND_TEST(ImageChannel, OutputPixelForKernel);
public:
    Channel(unsigned int maxValue, const IEEE754_t* elements, unsigned int rows, unsigned int columns, MatrixLayout layout = ROW_MAJOR);
    Channel(unsigned int maxValue, const Matrix<IEEE754_t>* channelValues);

    [[nodiscard]] unsigned int getMaxTheoreticalValue() const;
    [[nodiscard]] Channel* normalized() const;

};

extern template class Channel<float>;
extern template class Channel<double>;
extern template class Channel<long double>;

#endif