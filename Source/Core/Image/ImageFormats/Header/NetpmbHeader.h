#ifndef IMAGECONVOLUTIONKERNEL_NETPMBHEADER_H
#define IMAGECONVOLUTIONKERNEL_NETPMBHEADER_H

#include <optional>

#include "../../Image.h"

class NetpmbHeader {
private:
    ImageNetpbmFormat format;
    unsigned int rows;
    unsigned int columns;
    std::streampos positionOfFirstPixel;
    static std::optional<std::string> getNextWord(std::ifstream& fromFile);

protected:
    NetpmbHeader(ImageNetpbmFormat format, unsigned int rows, unsigned int columns, const std::streampos& positionOfFirstPixel);

public:
    [[nodiscard]] ImageNetpbmFormat getFormat() const;
    [[nodiscard]] unsigned int getRows() const;
    [[nodiscard]] unsigned int getColumns() const;
    [[nodiscard]] std::streampos getPositionOfFirstPixel() const;

    static NetpmbHeader* parsing(const std::filesystem::path& filePath);
};


#endif