#ifndef IMAGECONVOLUTIONKERNEL_NETPMBHEADER_H
#define IMAGECONVOLUTIONKERNEL_NETPMBHEADER_H

#include <optional>

#include "../../Image.h"

class NetpbmHeader {
private:
    ImageNetpbmFormat format;
    unsigned int rows;
    unsigned int columns;
    unsigned int maxPixelValue;
    std::streampos positionOfFirstPixel;

protected:
    NetpbmHeader(ImageNetpbmFormat format, unsigned int rows, unsigned int columns, unsigned int max, const std::streampos& positionOfFirstPixel);

public:
    [[nodiscard]] ImageNetpbmFormat getFormat() const;
    [[nodiscard]] unsigned int getRows() const;
    [[nodiscard]] unsigned int getColumns() const;
    [[nodiscard]] unsigned int getMaxPixelValue() const;
    [[nodiscard]] std::streampos getPositionOfFirstPixel() const;

    static NetpbmHeader* parsing(const std::filesystem::path& filePath);
};


#endif