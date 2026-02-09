#include <gtest/gtest.h>
#include  "../../Source/Core/ConvolutionKernel/ConvolutionKernel.h"


TEST(ConvolutionKernelTests, CentralIndexOddRowMajor) {
    float gaussianBlurKernelValues[25] = {
        1.0f/256,  4.0f/256,  6.0f/256,  4.0f/256, 1.0f/256,
        4.0f/256, 16.0f/256, 24.0f/256, 16.0f/256, 4.0f/256,
        6.0f/256, 24.0f/256, 36.0f/256, 24.0f/256, 6.0f/256,
        4.0f/256, 16.0f/256, 24.0f/256, 16.0f/256, 4.0f/256,
        1.0f/256,  4.0f/256,  6.0f/256,  4.0f/256, 1.0f/256
    };

    auto gaussianBlur = new ConvolutionKernel<float>(
        gaussianBlurKernelValues,
        5,
        5,
        ROW_MAJOR
    );

    EXPECT_EQ(gaussianBlur->getCentralRowIndex(), 2);
    EXPECT_EQ(gaussianBlur->getCentralColumnIndex(), 2);
}


/*
 * EXAMPLE WITH 4x4 KERNEL:
 *      Row 0:  0.0033  0.0149  0.0245  0.0149
 *      Row 1:  0.0149  0.0667  0.1100  0.0667
 *      Row 2:  0.0245  0.1100  0.1813* 0.1100  ← Central row element
 *      Row 3:  0.0149  0.0667  0.1100  0.0667
 *                        ↑
 *                  Central column element
*/
TEST(ConvolutionKernelTests, CentralIndexEvenRowMajor) {
    float gaussianBlurKernelValues[36] = {
        0.0019f, 0.0050f, 0.0095f, 0.0131f, 0.0131f, 0.0095f,
        0.0050f, 0.0131f, 0.0248f, 0.0342f, 0.0342f, 0.0248f,
        0.0095f, 0.0248f, 0.0469f, 0.0647f, 0.0647f, 0.0469f,
        0.0131f, 0.0342f, 0.0647f, 0.0893f, 0.0893f, 0.0647f,
        0.0131f, 0.0342f, 0.0647f, 0.0893f, 0.0893f, 0.0647f,
        0.0095f, 0.0248f, 0.0469f, 0.0647f, 0.0647f, 0.0469f
    };

    auto gaussianBlur = new ConvolutionKernel<float>(
        gaussianBlurKernelValues,
        6,
        6,
        ROW_MAJOR
    );

    EXPECT_EQ(gaussianBlur->getCentralRowIndex(), 3);
    EXPECT_EQ(gaussianBlur->getCentralColumnIndex(), 3);
}



TEST(ConvolutionKernelTests, LowerAndUpperBoundsOddSizeKernel) {
    float gaussianBlurKernelValues[25] = {
        1.0f/256,  4.0f/256,  6.0f/256,  4.0f/256, 1.0f/256,
        4.0f/256, 16.0f/256, 24.0f/256, 16.0f/256, 4.0f/256,
        6.0f/256, 24.0f/256, 36.0f/256, 24.0f/256, 6.0f/256,
        4.0f/256, 16.0f/256, 24.0f/256, 16.0f/256, 4.0f/256,
        1.0f/256,  4.0f/256,  6.0f/256,  4.0f/256, 1.0f/256
    };

    auto gaussianBlur = new ConvolutionKernel<float>(
        gaussianBlurKernelValues,
        5,
        5,
        ROW_MAJOR
    );

    EXPECT_EQ(gaussianBlur->getLowerBoundRowIndex(), -2);
    EXPECT_EQ(gaussianBlur->getUpperBoundRowIndex(), 2);
    EXPECT_EQ(gaussianBlur->getUpperBoundRowIndex() - gaussianBlur->getLowerBoundRowIndex() + 1, 5);

    EXPECT_EQ(gaussianBlur->getLowerBoundColumnIndex(), -2);
    EXPECT_EQ(gaussianBlur->getUpperBoundColumnIndex(), 2);
    EXPECT_EQ(gaussianBlur->getUpperBoundColumnIndex() - gaussianBlur->getLowerBoundRowIndex() + 1, 5);
}

/*
 * EXAMPLE WITH 4x4 KERNEL:
 *     Row -2:  0.0033  0.0149  0.0245  0.0149
 *     Row -1:  0.0149  0.0667  0.1100  0.0667
 *      Row 0:  0.0245  0.1100  0.1813* 0.1100  ← Central row element
 *      Row 1:  0.0149  0.0667  0.1100  0.0667
 *                        ↑
 *                  Central column element
*/
TEST(ConvolutionKernelTests, LowerAndUpperBoundsEvenSizeKernel) {
    float gaussianBlurKernelValues[36] = {
        0.0019f, 0.0050f, 0.0095f, 0.0131f, 0.0131f, 0.0095f,
        0.0050f, 0.0131f, 0.0248f, 0.0342f, 0.0342f, 0.0248f,
        0.0095f, 0.0248f, 0.0469f, 0.0647f, 0.0647f, 0.0469f,
        0.0131f, 0.0342f, 0.0647f, 0.0893f, 0.0893f, 0.0647f,
        0.0131f, 0.0342f, 0.0647f, 0.0893f, 0.0893f, 0.0647f,
        0.0095f, 0.0248f, 0.0469f, 0.0647f, 0.0647f, 0.0469f
    };

    auto gaussianBlur = new ConvolutionKernel<float>(
        gaussianBlurKernelValues,
        6,
        6,
        ROW_MAJOR
    );

    EXPECT_EQ(gaussianBlur->getLowerBoundRowIndex(), -2);
    EXPECT_EQ(gaussianBlur->getUpperBoundRowIndex(), 1);
    EXPECT_EQ(gaussianBlur->getUpperBoundRowIndex() - gaussianBlur->getLowerBoundRowIndex() + 1, 4);
}
