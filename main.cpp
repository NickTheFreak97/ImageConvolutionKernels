#include "Source/Core/Image/ImageFormats/PPM/PPMImage.h"
#include "Source/Core/ConvolutionKernel/Kernels/GaussianKernel.cpp"
#include "Source/Core/MatrixPaddingStrategy/PeriodicExtensionMatrixPaddingStrategy/PeriodicExtensionMatrixPaddingStrategy.h"
#include <filesystem>

int main() {
    auto loadedImage = NetpbmImage<float, PPMImage<float>>::loadImage( std::filesystem::current_path() / "paw.ppm");
    auto gaussianBlur = Kernels::gaussianKernel<float>(9, 1.3);
    auto imagePaddingStrategy = new PeriodicExtensionMatrixPaddingStrategy<float>();

    auto blurredImage = loadedImage->filtered(gaussianBlur, imagePaddingStrategy);
    blurredImage->writeToFile(std::filesystem::current_path() /"paw_blurred.ppm", ImageChannelsEncoding::BINARY);
    return 0;
}
