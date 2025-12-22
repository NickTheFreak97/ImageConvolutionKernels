#ifndef IMAGECONVOLUTIONKERNEL_MATRIX_H
#define IMAGECONVOLUTIONKERNEL_MATRIX_H

#include <type_traits>

enum MatrixLayout {
    ROW_MAJOR,
    COLUMN_MAJOR
};

template <typename IEEE754_t>
    requires std::is_floating_point_v<IEEE754_t>
class Matrix {
private:
    IEEE754_t* matrix;
    MatrixLayout layout;
    unsigned int rows;
    unsigned int columns;

public:
    Matrix(const IEEE754_t* elements, unsigned int rows, unsigned int columns, MatrixLayout layout = ROW_MAJOR);

    static Matrix<IEEE754_t> random(unsigned int, unsigned int, MatrixLayout layout = ROW_MAJOR);

    IEEE754_t* operator[](int row) const;

    Matrix<IEEE754_t> transposed() const;

    MatrixLayout getMatrixLayout() const;
    unsigned int getRows() const;
    unsigned int getColumns() const;
    IEEE754_t at(unsigned int, unsigned int) const;

    ~Matrix();
};

extern template class Matrix<float>;
extern template class Matrix<double>;
extern template class Matrix<long double>;

#endif
