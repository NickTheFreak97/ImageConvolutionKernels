#include <gtest/gtest.h>
#include <iostream>
#include <map>
#include <random>
#include  "../../Source/Core/Matrix/Matrix.h"


TEST(MatrixTest, Transposition) {
    auto rows = rand() % 100 + 1;
    auto columns = rand() % 100 + 1;

    auto randomElements = new double[rows * columns];

    std::random_device rd;
    std::mt19937 e2(rd());

    std::uniform_real_distribution<> dist(0, 100);
    for (int i = 0; i < rows * columns; i++) {
        randomElements[i] = dist(e2);
    }

    Matrix<double> rowMajorMatrix = Matrix<double>(
        randomElements,
        rows,
        columns,
        COLUMN_MAJOR
    );


    auto itsTranspose = rowMajorMatrix.transposed();
    for (int i=0; i < 4; i++) {
        for (int j=0; j < 3; j++) {
            EXPECT_EQ(rowMajorMatrix[i][j], itsTranspose[j][i]);
        }
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
