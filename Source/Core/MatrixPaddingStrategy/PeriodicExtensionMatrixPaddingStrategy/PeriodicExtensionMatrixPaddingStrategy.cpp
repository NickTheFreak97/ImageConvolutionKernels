#include <type_traits>

#include "PeriodicExtensionMatrixPaddingStrategy.h"

#include <cmath>

/*
 * This padding strategy implements periodic extension of the sourced matrix.
 *
 * Its main advantage is that it preserves continuity of the matrix. The padding is performed as follows: we horizontally repeat the matrix infinitely, and vertically as well. Example:
```
 | a    b   c |
 | d    e   f |
 | g    h   i |

 Is extended as follows

i   h   g   g    h   i   i  h   g
f   e   d   d    e   f   f  e   d
c   b   a   a    b   c   c  b   a
c   b   a | a    b   c | c  b   a
f   e   d | d    e   f | f  e   d
i   h   g | g    h   i | i  h   g
i   h   g   g    h   i   i  h   g
f   e   d   d    e   f   f  e   d
c   b   a   a    b   c   c  b   a
```

 * We construct vertical and horizontal repetitions of the input matrix.
 */
template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
IEEE754_t PeriodicExtensionMatrixPaddingStrategy<IEEE754_t>::pad(
    const Matrix<IEEE754_t> & matrix,
    int row,
    int column
) const {
    if (row >= 0 && row < matrix.getRows() && column >= 0 && column < matrix.getColumns()) {
        return matrix.at(row, column);
    } else {
        bool isHorizontallyReflected = column >= 0 ?
            static_cast<int>(
                floor(
                    static_cast<double>(column)/static_cast<double>(matrix.getColumns())
                 )
            ) % 2 > 0
                :
            static_cast<int>(
                floor(
                    -static_cast<double>(column + 1)/static_cast<double>(matrix.getColumns())
                    )
            ) % 2 > 0;

        bool isVerticallyReflected = row >= 0 ?
            static_cast<int>(
                floor(
                    static_cast<double>(row)/static_cast<double>(matrix.getRows())
                )
             ) % 2 > 0
                :
            static_cast<int>(
                floor(
                    -static_cast<double>(row + 1)/static_cast<double>(matrix.getRows())
                    )
            ) % 2 > 0;

        auto normalizedRowIndex = row >= 0 ?
            row % matrix.getRows()
                :
            (-row - 1) % matrix.getRows();
        auto normalizedColumnIndex = column >= 0 ?
            column % matrix.getColumns()
                :
            (-column - 1) % matrix.getColumns();

        return matrix.at(
            isVerticallyReflected ? matrix.getRows() - 1 - normalizedRowIndex : normalizedRowIndex,
            isHorizontallyReflected ? matrix.getColumns() - 1 - normalizedColumnIndex : normalizedColumnIndex
        );
    }
}

template class PeriodicExtensionMatrixPaddingStrategy<float>;
template class PeriodicExtensionMatrixPaddingStrategy<double>;
template class PeriodicExtensionMatrixPaddingStrategy<long double>;
