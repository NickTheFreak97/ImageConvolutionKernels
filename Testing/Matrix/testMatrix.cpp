#include <gtest/gtest.h>
#include  "../../Source/Core/Matrix/Matrix.h"
#include "../../Source/Core/MatrixPaddingStrategy/MatrixPaddingStrategy.h"
#include "../../Source/Core/MatrixPaddingStrategy/PeriodicExtensionMatrixPaddingStrategy/PeriodicExtensionMatrixPaddingStrategy.h"
#include "../../Source/Core/MatrixPaddingStrategy/Zero Padding/ZeroPaddingMatrixPaddingStrategy.h"


TEST(MatrixTest, Transposition) {
    auto rows = rand() % 100 + 1;
    auto columns = rand() % 100 + 1;

    auto rowMajorMatrix = Matrix<double>::random(rows, columns);

    auto itsTranspose = rowMajorMatrix.transposed();
    for (int i=0; i < rows; i++) {
        for (int j=0; j < columns; j++) {
            EXPECT_EQ(rowMajorMatrix[i][j], itsTranspose[j][i]);
        }
    }
}

TEST(MatrixTest, AtFunction) {
    auto rows = rand() % 100 + 1;
    auto columns = rand() % 100 + 1;

    auto rowMajorMatrix =  Matrix<double>::random(rows, columns, COLUMN_MAJOR);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            EXPECT_EQ(rowMajorMatrix[i][j], rowMajorMatrix.at(i, j));
        }
    }
}

TEST(MatrixTest, ZeroPadding) {
    MatrixPaddingStrategy<double>* strategy = new ZeroPaddingMatrixPaddingStrategy<double>();

    auto rows = rand() % 100 + 1;
    auto columns = rand() % 100 + 1;

    auto rowMajorMatrix = Matrix<double>::random(rows, columns, COLUMN_MAJOR);

    for (int i = 0; i < 100; i++) {
        auto randomRowIndex = rand() % 400 - 200;
        auto randomColumnIndex = rand() % 400 - 200;

        if (randomColumnIndex >= 0 && randomColumnIndex < rowMajorMatrix.getColumns() &&
            randomRowIndex >= 0 && randomRowIndex < rowMajorMatrix.getRows()) {
            EXPECT_EQ(
                rowMajorMatrix[randomRowIndex][randomColumnIndex],
                strategy->pad(rowMajorMatrix, randomRowIndex, randomColumnIndex)
            );
        } else {
            EXPECT_EQ(
                strategy->pad(rowMajorMatrix, randomRowIndex, randomColumnIndex),
                0
            );
        }
    }
}

/*
 * FIXME:  Actually this is not a test case. It just graphically shows that it works.
 *
 * A possible strategy to display that the strategy works as expected would be to write test cases for
 * North, East, South, West boundaries to show continuity, then generate a number random indices,
 * establish if they touch a reversed copy of the matrix or not, then confirm that the output of `pad`
 * matches expectation. Likely deserves its own test suite.
 */
TEST(MatrixTest, MirroringPadding) {
    MatrixPaddingStrategy<double>* strategy = new PeriodicExtensionMatrixPaddingStrategy<double>();

    auto rowMajorMatrix = Matrix<double>::random(3, 3, COLUMN_MAJOR);

    for ( int i = 0; i < 3; i ++) {
        for ( int j = 0; j < 3; j++ ) {
            std::cout << strategy->pad(rowMajorMatrix, i, j) << " ";
        }

        std::cout << std::endl;
    }

    std::cout << "===========" << std::endl;


    for ( int i = -6; i < 9; i ++) {
        for ( int j = -6; j < 9; j++ ) {
            if ( j == 0 && i >= 0 && i < 3) {
                std::cout << " | ";
            }
            std::cout << strategy->pad(rowMajorMatrix, i, j) << " ";

            if ( j == 2 && i >= 0 && i < 3 ) {
                std::cout << "| ";
            }
        }

        std::cout << std::endl;
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
