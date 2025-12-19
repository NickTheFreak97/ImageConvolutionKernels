#include "ZeroPaddingMatrixPaddingStrategy.h"
#include <type_traits>

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
    IEEE754_t ZeroPaddingMatrixPaddingStrategy<IEEE754_t>::pad(
        const Matrix<IEEE754_t>& matrix,
        int row,
        int column
    ) const {
        if ( row >= 0 && row < matrix.getRows() &&
                column >= 0 && column < matrix.getColumns() ) {
            return matrix.at(row, column);
        } else {
            return 0;
        }
    }

template class ZeroPaddingMatrixPaddingStrategy<float>;
template class ZeroPaddingMatrixPaddingStrategy<double>;
template class ZeroPaddingMatrixPaddingStrategy<long double>;
