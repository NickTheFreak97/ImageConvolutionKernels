#ifndef IMAGECONVOLUTIONKERNEL_CHANNEL_H
#define IMAGECONVOLUTIONKERNEL_CHANNEL_H

#include <type_traits>
#include "../Matrix/Matrix.h"

template<typename IEEE754_t>
    requires std::is_floating_point_v<IEEE754_t>
class Channel: public Matrix<IEEE754_t> {

private:
    unsigned int maxTheoreticalValue;
    bool isWithinMaxThreshold();

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