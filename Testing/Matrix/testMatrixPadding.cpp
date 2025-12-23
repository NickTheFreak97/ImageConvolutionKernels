#include <gtest/gtest.h>
#include <limits>
#include <random>

#include  "../../Source/Core/Matrix/Matrix.h"
#include "../../Source/Core/MatrixPaddingStrategy/MatrixPaddingStrategy.h"
#include "../../Source/Core/MatrixPaddingStrategy/PeriodicExtensionMatrixPaddingStrategy/PeriodicExtensionMatrixPaddingStrategy.h"


TEST(periodicExtensionPadding, northernBoundary) {
    // Should test that the periodic extension is continuous at its northern boundary
    std::random_device rd;
    std::uniform_real_distribution<double> urdForMatrixSize(1, 100);

    auto randomRows = static_cast<int>(urdForMatrixSize(rd));
    auto randomColumns = static_cast<int>(urdForMatrixSize(rd));


    auto randomMatrix = Matrix<double>::random(randomRows, randomColumns);
    MatrixPaddingStrategy<double>* strategy = new PeriodicExtensionMatrixPaddingStrategy<double>();

    for (int j = 0; j < randomColumns; j++) {
        EXPECT_DOUBLE_EQ(strategy->pad(randomMatrix, -1, j), randomMatrix.at(0, j));
    }
}


TEST(periodicExtensionPadding, northernBoundaryExtension) {
    // Should test that the periodic extension is continuous at its northern boundary for an arbitrary offset.
    // Idea is that I generate a random negative integer number, then divide its absolute by the number of columns rows of the matrix:
    // If the floor of the division is even then I should find a reflected copy of the matrix, else, I should be dealing with an exact copy of the matrix.
    // In testing boundary of repetitions keep in mind that indices of rows are (-1)-based and not 0-based.

    MatrixPaddingStrategy<double>* strategy = new PeriodicExtensionMatrixPaddingStrategy<double>();

    std::random_device rd;
    std::uniform_real_distribution<double> dist(-(std::numeric_limits<int>::max() - 1), -1);
    std::uniform_real_distribution<double> urdForMatrixSize(10, 100);

    auto randomRows = static_cast<int>(urdForMatrixSize(rd));
    auto randomColumns = static_cast<int>(urdForMatrixSize(rd));

    auto randomMatrix = Matrix<double>::random(randomRows, randomColumns);

    for ( int k = 0; k < 50'000; k ++) {
        auto someRandomRow = round(dist(rd));
        auto numberOfRepetition = floor(-someRandomRow / static_cast<double>(randomRows));

        if (static_cast<int>(someRandomRow) % randomRows == 0) {
            numberOfRepetition--;
        }

        for ( int j = 0; j < randomColumns; j++) {
            auto projectedRowIndex = -someRandomRow - floor(-someRandomRow / randomRows) * randomRows;

            if (static_cast<int>(numberOfRepetition) % 2 == 0) {
                EXPECT_DOUBLE_EQ(strategy->pad(randomMatrix, someRandomRow, j), randomMatrix.at((randomRows - 1 - projectedRowIndex) * (static_cast<int>(-someRandomRow) % randomRows == 0) + (projectedRowIndex - 1) * (static_cast<int>(-someRandomRow) % randomRows != 0), j));
            } else {
                EXPECT_DOUBLE_EQ(strategy->pad(randomMatrix, someRandomRow, j), randomMatrix.at(static_cast<int>(projectedRowIndex) * (static_cast<int>(-someRandomRow) % randomRows == 0) + (static_cast<int>(randomRows - projectedRowIndex)) * (static_cast<int>(-someRandomRow) % randomRows != 0), j));
            }
        }
    }

    for (int j = 0; j < randomColumns; j++) {
        EXPECT_DOUBLE_EQ(strategy->pad(randomMatrix, -1, j), randomMatrix.at(0, j));
    }
}

TEST(periodicExtensionPadding, NorthernBoundPeriodicity) {
    // Starting from index 0, the periodic extension padding is expected to be vertically periodic, with period equal to twice the number of columns of the matrix.
    // That is, given a random k∈ℕ, and a matrix A ∈ Matrix(ℝ, r * c) the padding is periodic with period 2kc on the northern boundary.

    MatrixPaddingStrategy<double>* strategy = new PeriodicExtensionMatrixPaddingStrategy<double>();

    std::random_device rd;
    std::uniform_int_distribution urdForMatrixSize(10, 100);

    auto randomRows = urdForMatrixSize(rd);
    auto randomColumns = urdForMatrixSize(rd);

    auto randomMatrix = Matrix<double>::random(randomRows, randomColumns);

    std::uniform_int_distribution urdForRandomRow(0, randomRows - 1);
    auto randomRowIndex = urdForRandomRow(rd);

    for ( int k = 0; k < 5'000; k++) {
        std::uniform_int_distribution urdForRandomPeriod(1, std::numeric_limits<int>::max() / (2 * randomRows) - 1);
        auto randomPeriod = urdForRandomPeriod(rd);

        for (int j = 0; j < randomColumns; j++) {
            EXPECT_DOUBLE_EQ(
                randomMatrix.at(randomRowIndex, j),
                strategy->pad(
                    randomMatrix,
                    randomRowIndex - 2*randomRows*randomPeriod,
                    j
                )
            );
        }
    }
}



TEST(periodicExtensionPadding, NorthernBoundContinuity) {
    // The extension is supposed to be continuous at the northern boundaries.
    // That is, given a random k∈ℕ, and a matrix A ∈ Matrix(ℝ, r * c) the padding's rows at indices 2kc and 2kc + 1 must be the same vector.

    MatrixPaddingStrategy<double>* strategy = new PeriodicExtensionMatrixPaddingStrategy<double>();

    std::random_device rd;
    std::uniform_int_distribution urdForMatrixSize(10, 100);

    auto randomRows = urdForMatrixSize(rd);
    auto randomColumns = urdForMatrixSize(rd);

    auto randomMatrix = Matrix<double>::random(randomRows, randomColumns);

    for ( int k = 0; k < 5'000; k++) {
        std::uniform_int_distribution urdForRandomPeriod(1, std::numeric_limits<int>::max() / (2 * randomRows) - 1);
        auto randomPeriod = urdForRandomPeriod(rd);

        for (int j = 0; j < randomColumns; j++) {
            EXPECT_DOUBLE_EQ(
                strategy->pad(
                    randomMatrix,
                    -2*randomRows*randomPeriod,
                    j
                ),

                strategy->pad(
                    randomMatrix,
                    -2*randomRows*randomPeriod - 1,
                    j
                )
            );
        }
    }
}