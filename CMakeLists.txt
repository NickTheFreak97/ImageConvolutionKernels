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
)

target_link_libraries(ImageConvolutionKernel PRIVATE gtest gtest_main)

add_executable(
        tests
        Source/Core/Matrix/Matrix.cpp
        Source/Core/Matrix/Matrix.h
        Testing/Matrix/testMatrix.cpp
)

target_link_libraries(tests PRIVATE GTest::gtest GTest::gtest_main)

add_test(NAME TestTest COMMAND tests)