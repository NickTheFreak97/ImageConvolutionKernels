//
// Created by Niccolo Della Rocca on 07/02/26.
//

#include "ConvolutionKernel.h"

#include <cmath>
#include <sys/stat.h>

template <typename IEEE754_t>
    requires std::is_floating_point_v<IEEE754_t>
ConvolutionKernel<IEEE754_t>::ConvolutionKernel(const IEEE754_t *elements, unsigned int rows, unsigned int columns, MatrixLayout layout) : Matrix<IEEE754_t>(elements, rows, columns, layout) {

}

/*
 * Since by design I decided to allow kernels of any size, to define the central row index we define the following rule:
 * - If the number of rows is odd, the central index is clearly `rowsCount / 2`, in the sense of integer division (rounded down)
 * - If the number of rows is even, the central row index is conventionally selected as `ceil((rowsCount-1)/2)`
 */
template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
int ConvolutionKernel<IEEE754_t>::getCentralRowIndex() {
    auto rows = static_cast<int>(this->getRows());

    return rows % 2 == 0 ? static_cast<int>(ceil(static_cast<float>(rows - 1) / 2)) : rows / 2;
}

/*
 * Since by design I decided to allow kernels of any size, to define the central row index we define the following rule:
 * - If the number of columns is odd, the central index is clearly `columnsCount / 2`, in the sense of integer division (rounded down)
 * - If the number of columns is even, the central column index is conventionally selected as `ceil((columnsCount-1)/2)`
 */
template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
int ConvolutionKernel<IEEE754_t>::getCentralColumnIndex() {
    auto columns = static_cast<int>(this->getColumns());
    return columns % 2 == 0 ? static_cast<int>(ceil(static_cast<float>(columns - 1) / 2)) : columns / 2;
}

/*
 * We use this method to define an indexing system for rows where the central element has index 0:
 * - If the number of columns is odd, the greatest distance on the left from the central row element is `-(rowsCount - 1) / 2`, that is guaranteed to be even.
 * - If the number of columns is even, the greatest distance on the left from the central row element is `-ceil((rowsCount - 1) / 2)`
 */
template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
int ConvolutionKernel<IEEE754_t>::getLowerBoundRowIndex() {
    auto rows = static_cast<int>(this->getRows());

    return rows % 2 == 0 ? -static_cast<int>(ceil(static_cast<float>(rows - 1) / 2)) : -(rows - 1) / 2;
}

/*
 * We use this method to define an indexing system for rows where the central element has index 0:
 * - If the number of rows is odd, the greatest distance on the left from the central row element `(rowsCount - 1) / 2`, that is guaranteed to be even.
 * - If the number of rows is even, the greatest distance on the left from the central row element is `ceil((rowsCount - 1) / 2) - 1`
 */
template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
int ConvolutionKernel<IEEE754_t>::getUpperBoundRowIndex() {
    auto rows = static_cast<int>(this->getRows());

    return rows % 2 == 0 ? static_cast<int>(ceil(static_cast<float>(rows - 1) / 2)) - 1 : (rows - 1) / 2;
}


/*
 * We use this method to define an indexing system for rows where the central column element has index 0:
 * - If the number of columns is odd, the greatest distance on the left from the central column element is `-(columnsCount - 1) / 2`, that is guaranteed to be even.
 * - If the number of columns is even, the greatest distance on the left from the central column element is `-ceil((columnsCount - 1) / 2)`
 */
template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
int ConvolutionKernel<IEEE754_t>::getLowerBoundColumnIndex() {
    auto columns = static_cast<int>(this->getColumns());

    return columns % 2 == 0 ? -static_cast<int>(ceil(static_cast<float>(columns - 1) / 2)) : -(columns - 1) / 2;
}

/*
 * We use this method to define an indexing system for rows where the central column element has index 0:
 * - If the number of columns is odd, the greatest distance on the left from the central column element `(columnsCount - 1) / 2`, that is guaranteed to be even.
 * - If the number of columns is even, the greatest distance on the left from the central column element is `ceil((columnsCount - 1) / 2) - 1`
 */
template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
int ConvolutionKernel<IEEE754_t>::getUpperBoundColumnIndex() {
    auto columns = static_cast<int>(this->getColumns());

    return columns % 2 == 0 ? static_cast<int>(ceil(static_cast<float>(columns - 1) / 2)) - 1 : (columns - 1) / 2;
}


template class ConvolutionKernel<float>;
template class ConvolutionKernel<double>;
template class ConvolutionKernel<long double>;
