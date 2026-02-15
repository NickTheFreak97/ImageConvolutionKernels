
#ifndef IMAGECONVOLUTIONKERNEL_FILEUTILS_H
#define IMAGECONVOLUTIONKERNEL_FILEUTILS_H

#include <optional>
#include <string>
#include <filesystem>

class FileUtils {
public:
    static std::optional<std::string> getNextWord(std::ifstream& fromFile);
    static std::optional<std::string> getNextBytes(std::ifstream& fromFile, unsigned int requestedBytes);
};


#endif