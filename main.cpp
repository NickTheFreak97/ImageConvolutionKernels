#include <iostream>
#include "Source/Core/Matrix/Matrix.h"

int main() {
    float matrixData[] = {1.0f, 2.0f, 3.0f, 4.0f};
    Matrix<float> mat(matrixData, 2, 2, ROW_MAJOR);

    return 0;
}