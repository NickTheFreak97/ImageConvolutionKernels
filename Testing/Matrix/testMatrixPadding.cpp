#include <gtest/gtest.h>
#include <limits>
#include <random>

#include  "../../Source/Core/Matrix/Matrix.h"
#include "../../Source/Core/MatrixPaddingStrategy/MatrixPaddingStrategy.h"
#include "../../Source/Core/MatrixPaddingStrategy/PeriodicExtensionMatrixPaddingStrategy/PeriodicExtensionMatrixPaddingStrategy.h"

// NORTHERN BOUNDARY
TEST(periodicExtensionPadding, northernBoundary) {
    // Should test that the periodic extension is continuous at its northern boundary
    std::random_device rd;
    std::uniform_int_distribution urdForMatrixSize(1, 100);

    auto randomRows = urdForMatrixSize(rd);
    auto randomColumns = urdForMatrixSize(rd);


    auto randomMatrix = Matrix<double>::random(randomRows, randomColumns);
    MatrixPaddingStrategy<double>* strategy = new PeriodicExtensionMatrixPaddingStrategy<double>();

    for (int j = 0; j < randomColumns; j++) {
        EXPECT_DOUBLE_EQ(strategy->pad(randomMatrix, -1, j), randomMatrix.at(0, j));
    }

    delete strategy;
}


TEST(periodicExtensionPadding, northernBoundaryExtension) {
    // Should test that the periodic extension is continuous at its northern boundary for an arbitrary offset.
    // Idea is that I generate a random negative integer number, then divide its absolute by the number of columns rows of the matrix:
    // If the floor of the division is even then I should find a reflected copy of the matrix, else, I should be dealing with an exact copy of the matrix.
    // In testing boundary of repetitions keep in mind that indices of rows are (-1)-based and not 0-based.

    MatrixPaddingStrategy<double>* strategy = new PeriodicExtensionMatrixPaddingStrategy<double>();

    std::random_device rd;
    std::uniform_int_distribution dist(-(std::numeric_limits<int>::max() - 1), -1);
    std::uniform_int_distribution urdForMatrixSize(10, 100);

    auto randomRows = urdForMatrixSize(rd);
    auto randomColumns = urdForMatrixSize(rd);

    auto randomMatrix = Matrix<double>::random(randomRows, randomColumns);

    for ( int k = 0; k < 50'000; k ++) {
        auto someRandomRow = dist(rd);
        auto numberOfRepetition = floor(-someRandomRow / static_cast<double>(randomRows));

        if (someRandomRow % randomRows == 0) {
            numberOfRepetition--;
        }

        for ( int j = 0; j < randomColumns; j++) {
            auto projectedRowIndex = static_cast<int>(-static_cast<double>(someRandomRow) - floor(-static_cast<double>(someRandomRow) / static_cast<double>(randomRows)) * static_cast<double>(randomRows));

            if (static_cast<int>(numberOfRepetition) % 2 == 0) {
                EXPECT_DOUBLE_EQ(strategy->pad(randomMatrix, someRandomRow, j), randomMatrix.at((randomRows - 1 - projectedRowIndex) * ((-someRandomRow) % randomRows == 0) + (projectedRowIndex - 1) * ((-someRandomRow) % randomRows != 0), j));
            } else {
                EXPECT_DOUBLE_EQ(strategy->pad(randomMatrix, someRandomRow, j), randomMatrix.at(projectedRowIndex * ((-someRandomRow) % randomRows == 0) + ((randomRows - projectedRowIndex)) * ((-someRandomRow) % randomRows != 0), j));
            }
        }
    }

    for (int j = 0; j < randomColumns; j++) {
        EXPECT_DOUBLE_EQ(strategy->pad(randomMatrix, -1, j), randomMatrix.at(0, j));
    }

    delete strategy;
}

TEST(periodicExtensionPadding, NorthernBoundPeriodicity) {
    // Starting from index 0, the periodic extension padding is expected to be vertically periodic, with period equal to twice the number of rows of the matrix.
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

    delete strategy;
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

    delete strategy;
}


// WESTERN BOUNDARY
TEST(periodicExtensionPadding, WesternBoundPeriodicity) {
    // Starting from index 0, the periodic extension padding is expected to be horizontally periodic to the left, with period equal to twice the number of columns of the matrix.
    // That is, given a random k∈ℕ, and a matrix A ∈ Matrix(ℝ, r * c) the padding is periodic with period 2kc on the northern boundary.

    auto matrixSizeDistribution = std::uniform_int_distribution(1, 100);
    auto rd = std::random_device();
    auto rows = matrixSizeDistribution(rd);
    auto columns = matrixSizeDistribution(rd);

    auto randomMatrix = Matrix<double>::random(rows, columns);

    auto randomColumnDistribution = std::uniform_int_distribution<int>(-(std::numeric_limits<int>::max() / columns - 1), -5);

    MatrixPaddingStrategy<double>* paddingStrategy = new PeriodicExtensionMatrixPaddingStrategy<double>();

    for (int k = 0; k < 5'000; k++) {
        auto randomWesternColumn = randomColumnDistribution(rd);
        auto repetition = static_cast<int>(floor(static_cast<double>((-randomWesternColumn) - 1)/ static_cast<double>(columns)) * ((-randomWesternColumn) % columns != 0) + (floor(static_cast<double>(-randomWesternColumn) / static_cast<double>(columns)) - 1) * ((-randomWesternColumn) % columns == 0));
        auto projectedColIndex = -randomWesternColumn - repetition * columns;

        if (repetition % 2 == 0) {
            for (int i = 0; i < rows; i++) {
                EXPECT_DOUBLE_EQ(
                    randomMatrix.at(i, projectedColIndex - 1),
                    paddingStrategy->pad(randomMatrix, i, randomWesternColumn)
                );
            }
        } else {
            for (int i = 0; i < rows; i++) {

                EXPECT_DOUBLE_EQ(
                    randomMatrix.at(i, columns - projectedColIndex),
                    paddingStrategy->pad(randomMatrix, i, randomWesternColumn)
                );
            }
        }
    }

    delete paddingStrategy;
}


TEST(periodicExtensionPadding, WesternBoundContinuity) {
    // The extension is supposed to be continuous at the western boundary.
    // That is, given a random k∈ℕ, and a matrix A ∈ Matrix(ℝ, r * c) the padding's column at indices 2kr and 2kr + 1 must be the same vector.

    MatrixPaddingStrategy<double>* strategy = new PeriodicExtensionMatrixPaddingStrategy<double>();

    std::random_device rd;
    std::uniform_int_distribution urdForMatrixSize(10, 100);

    auto randomRows = urdForMatrixSize(rd);
    auto randomColumn = urdForMatrixSize(rd);

    auto randomMatrix = Matrix<double>::random(randomRows, randomColumn);

    for ( int k = 0; k < 5'000; k++) {
        std::uniform_int_distribution urdForRandomPeriod(1, std::numeric_limits<int>::max() / (2 * randomColumn) - 1);
        auto randomPeriod = urdForRandomPeriod(rd);

        for (int i = 0; i < randomRows; i++) {
            EXPECT_DOUBLE_EQ(
                strategy->pad(
                    randomMatrix,
                    i,
                    -2*randomColumn*randomPeriod
                ),

                strategy->pad(
                    randomMatrix,
                    i,
                    -2*randomColumn*randomPeriod - 1
                )
            );
        }
    }

    delete strategy;
}