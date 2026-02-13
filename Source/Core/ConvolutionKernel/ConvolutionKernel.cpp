#include "ConvolutionKernel.h"

#include <cmath>
#include <cassert>

template <typename IEEE754_t>
    requires std::is_floating_point_v<IEEE754_t>
ConvolutionKernel<IEEE754_t>::ConvolutionKernel(const IEEE754_t *elements, unsigned int rows, unsigned int columns, MatrixLayout layout) : Matrix<IEEE754_t>(elements, rows, columns, layout) {

}


template <typename IEEE754_t>
    requires std::is_floating_point_v<IEEE754_t>
ConvolutionKernel<IEEE754_t>::ConvolutionKernel(const Matrix<IEEE754_t>* fromMatrix) : Matrix<IEEE754_t>(
    [fromMatrix] {
        assert(fromMatrix != nullptr);

        auto deepCopy = new IEEE754_t[fromMatrix->getRows() * fromMatrix->getColumns()];

        for (int i = 0; i < fromMatrix->getRows(); i++) {
            for (int j = 0; j < fromMatrix->getColumns(); j++) {
                if (fromMatrix->getMatrixLayout() == ROW_MAJOR) {
                    deepCopy[i * fromMatrix -> getRows() + j] = fromMatrix -> at(i, j);
                } else {
                    deepCopy[i + fromMatrix->getColumns() * j] = fromMatrix -> at(i, j);
                }
            }
        }

        return deepCopy;
    }(),
    fromMatrix->getRows(),
    fromMatrix->getColumns(),
    fromMatrix->getMatrixLayout()
) {

}

/*
 * Since by design I decided to allow kernels of any size, to define the central row index we define the following rule:
 * - If the number of rows is odd, the central index is clearly `rowsCount / 2`, in the sense of integer division (rounded down)
 * - If the number of rows is even, the central row index is conventionally selected as `ceil((rowsCount-1)/2)`
 */
template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
int ConvolutionKernel<IEEE754_t>::getCentralRowIndex() const {
    auto rows = static_cast<int>(this->getRows());

    return rows % 2 == 0 ? static_cast<int>(ceil(static_cast<float>(rows - 1) / 2)) : rows / 2;
}

/*
 * Since by design I decided to allow kernels of any size, to define the central row index we define the following rule:
 * - If the number of columns is odd, the central index is clearly `columnsCount / 2`, in the sense of integer division (rounded down)
 * - If the number of columns is even, the central column index is conventionally selected as `ceil((columnsCount-1)/2)`
 */
template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
int ConvolutionKernel<IEEE754_t>::getCentralColumnIndex() const {
    auto columns = static_cast<int>(this->getColumns());
    return columns % 2 == 0 ? static_cast<int>(ceil(static_cast<float>(columns - 1) / 2)) : columns / 2;
}

/*
 * We use this method to define an indexing system for rows where the central element has index 0:
 * - If the number of columns is odd, the greatest distance on the left from the central row element is `-(rowsCount - 1) / 2`, that is guaranteed to be even.
 * - If the number of columns is even, the greatest distance on the left from the central row element is `-ceil((rowsCount - 1) / 2)`
 */
template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
int ConvolutionKernel<IEEE754_t>::getLowerBoundRowIndex() const {
    auto rows = static_cast<int>(this->getRows());

    return rows % 2 == 0 ? -static_cast<int>(ceil(static_cast<float>(rows - 1) / 2)) : -(rows - 1) / 2;
}

/*
 * We use this method to define an indexing system for rows where the central element has index 0:
 * - If the number of rows is odd, the greatest distance on the left from the central row element `(rowsCount - 1) / 2`, that is guaranteed to be even.
 * - If the number of rows is even, the greatest distance on the left from the central row element is `ceil((rowsCount - 1) / 2) - 1`
 */
template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
int ConvolutionKernel<IEEE754_t>::getUpperBoundRowIndex() const {
    auto rows = static_cast<int>(this->getRows());

    return rows % 2 == 0 ? static_cast<int>(ceil(static_cast<float>(rows - 1) / 2)) - 1 : (rows - 1) / 2;
}


/*
 * We use this method to define an indexing system for rows where the central column element has index 0:
 * - If the number of columns is odd, the greatest distance on the left from the central column element is `-(columnsCount - 1) / 2`, that is guaranteed to be even.
 * - If the number of columns is even, the greatest distance on the left from the central column element is `-ceil((columnsCount - 1) / 2)`
 */
template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
int ConvolutionKernel<IEEE754_t>::getLowerBoundColumnIndex() const {
    auto columns = static_cast<int>(this->getColumns());

    return columns % 2 == 0 ? -static_cast<int>(ceil(static_cast<float>(columns - 1) / 2)) : -(columns - 1) / 2;
}

/*
 * We use this method to define an indexing system for rows where the central column element has index 0:
 * - If the number of columns is odd, the greatest distance on the left from the central column element `(columnsCount - 1) / 2`, that is guaranteed to be even.
 * - If the number of columns is even, the greatest distance on the left from the central column element is `ceil((columnsCount - 1) / 2) - 1`
 */
template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
int ConvolutionKernel<IEEE754_t>::getUpperBoundColumnIndex() const {
    auto columns = static_cast<int>(this->getColumns());

    return columns % 2 == 0 ? static_cast<int>(ceil(static_cast<float>(columns - 1) / 2)) - 1 : (columns - 1) / 2;
}

template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
ConvolutionKernel<IEEE754_t> *ConvolutionKernel<IEEE754_t>::averageKernel(unsigned int size) {
    auto elements = new IEEE754_t[size * size];

    for (int i = 0; i < size * size; i++) {
        elements[i] = 1.0/(size * size);
    }

    return new ConvolutionKernel(elements, size, size, ROW_MAJOR);
}


/*
 * G(x, y) = (1 / (2πσ²)) * exp(-(x² + y²) / (2σ²))
 * - x is the distance of the row index from the center of the kernel
 * - y is the distance of the column index from the center of the kernel
 * - Returns: A normalized kernel generated according to the above formula.
 *
 * - Note: A common choice is that σ is three times the width of the kernel in each direction, i.e. `size = 6*sigma`.
 * Another choice is `sigma = 0.3 * ((size - 1) * 0.5 - 1) + 0.8`, as used by OpenCV. Also for numerical stability sigma should be at least 0.5.
 */
template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
ConvolutionKernel<IEEE754_t> *ConvolutionKernel<IEEE754_t>::gaussianKernel(unsigned int size, IEEE754_t sigma) {
    auto elements = new IEEE754_t[size * size];
    auto maxValue = std::numeric_limits<IEEE754_t>::min();

    if (size % 2 != 0) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                auto x = (static_cast<IEEE754_t>(i) - static_cast<IEEE754_t>(size - 1) / 2);
                auto y = (static_cast<IEEE754_t>(j) - static_cast<IEEE754_t>(size - 1) / 2);

                elements[i * size + j] = 1.0 / (2 * M_PI * sigma * sigma) * exp(-(x*x + y*y) / (2*sigma * sigma));

                if (maxValue < elements[i * size + j]) {
                    maxValue = elements[i * size + j];
                }
            }
        }
    } else {

    }


    for (int i = 0; i < size * size; i++) {
        elements[i] /= maxValue;
    }

    return new ConvolutionKernel(elements, size, size, ROW_MAJOR);
}




/*
 * Returns the value of the kernel with respect to the specified `row` and `column` indices.
 * - Parameter row: An index s.t. the central row element of the kernel has index 0.
 * - Parameter column: An index s.t. the central column element of the kernel has index 0.
 * - Returns: The output of the kernel at the specified remapped indices.
 */
template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
IEEE754_t ConvolutionKernel<IEEE754_t>::getValue(int row, int column) const {
    assert(row >= this->getLowerBoundRowIndex() && row <= this->getUpperBoundRowIndex());
    assert(column >= this->getLowerBoundColumnIndex() && column <= this->getUpperBoundColumnIndex());

    auto mappedRowIndex = row + this->getCentralRowIndex();
    auto mappedColumnIndex = column + this->getCentralColumnIndex();

    return this->at(mappedRowIndex, mappedColumnIndex);
}


template class ConvolutionKernel<float>;
template class ConvolutionKernel<double>;
template class ConvolutionKernel<long double>;
