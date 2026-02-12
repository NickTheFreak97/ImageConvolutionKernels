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
)

target_link_libraries(ImageConvolutionKernel PRIVATE gtest gtest_main)

add_executable(
        tests
        Source/Core/Matrix/Matrix.cpp
        Source/Core/Matrix/Matrix.h
        Testing/Matrix/testMatrix.cpp
        Testing/Matrix/testMatrixPadding.cpp
        Testing/Image/testConvolutionKernel.cpp
        Source/Core/MatrixPaddingStrategy/MatrixPaddingStrategy.h
        "Source/Core/MatrixPaddingStrategy/Zero Padding/ZeroPaddingMatrixPaddingStrategy.cpp"
        "Source/Core/MatrixPaddingStrategy/Zero Padding/ZeroPaddingMatrixPaddingStrategy.h"
        Source/Core/MatrixPaddingStrategy/PeriodicExtensionMatrixPaddingStrategy/PeriodicExtensionMatrixPaddingStrategy.cpp
        Source/Core/MatrixPaddingStrategy/PeriodicExtensionMatrixPaddingStrategy/PeriodicExtensionMatrixPaddingStrategy.h
        Source/Core/ConvolutionKernel/ConvolutionKernel.cpp
        Source/Core/ConvolutionKernel/ConvolutionKernel.h
        Source/Core/Channel/Channel.cpp
        Source/Core/Channel/Channel.h
        Testing/Image/testConvolutionKernel.cpp
        Testing/Image/testImageChannels.cpp
        Testing/Image/testImageChannels.cpp
)

target_link_libraries(tests PRIVATE GTest::gtest GTest::gtest_main)

add_test(NAME TestTest COMMAND tests)