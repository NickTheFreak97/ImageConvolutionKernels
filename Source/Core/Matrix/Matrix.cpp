#include "Matrix.h"

#include <cassert>
#include <cstring>

template <typename IEEE754_t>
    requires std::is_floating_point_v<IEEE754_t>
Matrix<IEEE754_t>::Matrix(const IEEE754_t *elements, unsigned int rows, unsigned int columns, MatrixLayout layout) : rows(rows), columns(columns), layout(layout) {
    assert(rows > 0);
    assert(columns > 0);

    auto theMatrix = new IEEE754_t[rows * columns];
    std::memcpy(theMatrix, elements, rows * columns * sizeof(IEEE754_t));
    this->matrix = theMatrix;
}

template <typename IEEE754_t>
    requires std::is_floating_point_v<IEEE754_t>
IEEE754_t* Matrix<IEEE754_t>::operator[](int row) const {
    assert(row >= 0 && row < this->rows);

    auto requestedRow = new IEEE754_t[this->columns];
    memset(requestedRow, 0, sizeof(IEEE754_t) * this->columns);

    if (this->layout == ROW_MAJOR) {
        for (int i = 0; i < this->columns; i++) {
            requestedRow[i] = this->matrix[row * this->columns + i];
        }
    } else {
        for (int i = 0; i < this->columns; i++) {
            requestedRow[i] = this->matrix[row + this->rows * i];
        }
    }

    return requestedRow;
}


template<typename IEEE754_t> requires std::is_floating_point_v<IEEE754_t>
Matrix<IEEE754_t> Matrix<IEEE754_t>::transposed() const {
    auto transposedMatrix = new IEEE754_t[this->rows * this->columns];
    for ( int i = 0; i < this->rows * this->columns; i++ ) {
        int row = this->layout == ROW_MAJOR ? i / this->columns : i % this->rows;
        int col = this->layout == ROW_MAJOR ? i % this->columns : i / this->rows;

        auto transposedIndex = this->layout == ROW_MAJOR ? col * this->rows + row : col + row * this->columns;
        transposedMatrix[transposedIndex] = this->matrix[i];
    }

    return Matrix<IEEE754_t>(
        transposedMatrix,
        this->columns,
        this->rows,
        this->layout
    );
}


template <typename IEEE754_t>
    requires std::is_floating_point_v<IEEE754_t>
Matrix<IEEE754_t>::~Matrix() {
    delete[] this->matrix;
}

// Getters
template <typename IEEE754_t>
    requires std::is_floating_point_v<IEEE754_t>
MatrixLayout Matrix<IEEE754_t>::getMatrixLayout() const {
    return this->layout;
}

template <typename IEEE754_t>
    requires std::is_floating_point_v<IEEE754_t>
unsigned int Matrix<IEEE754_t>::getRows() const {
    return this->rows;
}


template <typename IEEE754_t>
    requires std::is_floating_point_v<IEEE754_t>
unsigned int Matrix<IEEE754_t>::getColumns() const {
    return this->columns;
}



template class Matrix<float>;
template class Matrix<double>;
template class Matrix<long double>;