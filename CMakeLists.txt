cmake_minimum_required(VERSION 3.25)

project(ImageConvolutionKernel)
set(CMAKE_CXX_STANDARD 26)

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        ImageConvolutionKernel
        main.cpp
        Source/Core/Matrix/Matrix.cpp
        Source/Core/Matrix/Matrix.h
        Source/Core/MatrixPaddingStrategy/MatrixPaddingStrategy.h
        "Source/Core/MatrixPaddingStrategy/Zero Padding/ZeroPaddingMatrixPaddingStrategy.cpp"
        "Source/Core/MatrixPaddingStrategy/Zero Padding/ZeroPaddingMatrixPaddingStrategy.h"
        Source/Core/MatrixPaddingStrategy/PeriodicExtensionMatrixPaddingStrategy/PeriodicExtensionMatrixPaddingStrategy.cpp
        Source/Core/MatrixPaddingStrategy/PeriodicExtensionMatrixPaddingStrategy/PeriodicExtensionMatrixPaddingStrategy.h
        Source/Core/ConvolutionKernel/ConvolutionKernel.cpp
        Source/Core/ConvolutionKernel/ConvolutionKernel.h
        Source/Core/Channel/Channel.cpp
        Source/Core/Channel/Channel.h
        Source/Core/ConvolutionKernel/Kernels/AverageKernel.cpp
        Source/Core/ConvolutionKernel/Kernels/GaussianKernel.cpp
        Source/Core/ConvolutionKernel/Kernels/Identity.cpp
        Source/Core/Image/Image.cpp
        Source/Core/Image/Image.h
        Source/Core/Image/ImageFormats/PPMImage.cpp
        Source/Core/Image/ImageFormats/PPMImage.h
)

target_link_libraries(ImageConvolutionKernel PRIVATE gtest gtest_main)

add_executable(
        tests
        Source/Core/Matrix/Matrix.cpp
        Source/Core/Matrix/Matrix.h
        Source/Core/MatrixPaddingStrategy/MatrixPaddingStrategy.h
        "Source/Core/MatrixPaddingStrategy/Zero Padding/ZeroPaddingMatrixPaddingStrategy.cpp"
        "Source/Core/MatrixPaddingStrategy/Zero Padding/ZeroPaddingMatrixPaddingStrategy.h"
        Source/Core/MatrixPaddingStrategy/PeriodicExtensionMatrixPaddingStrategy/PeriodicExtensionMatrixPaddingStrategy.cpp
        Source/Core/MatrixPaddingStrategy/PeriodicExtensionMatrixPaddingStrategy/PeriodicExtensionMatrixPaddingStrategy.h
        Source/Core/ConvolutionKernel/ConvolutionKernel.cpp
        Source/Core/ConvolutionKernel/ConvolutionKernel.h
        Source/Core/Channel/Channel.cpp
        Source/Core/Channel/Channel.h
        Source/Core/Image/Image.cpp
        Source/Core/Image/Image.h
        Source/Core/Image/ImageFormats/PPMImage.h
        Source/Core/Image/ImageFormats/PPMImage.cpp
        Testing/Matrix/testMatrix.cpp
        Testing/Matrix/testMatrixPadding.cpp
        Testing/Image/testConvolutionKernel.cpp
        Testing/Image/testImageChannels.cpp
        Testing/Image/testImageChannels.cpp
        Testing/Image/testImage.cpp
)

target_link_libraries(tests PRIVATE GTest::gtest GTest::gtest_main)

add_test(NAME TestTest COMMAND tests)