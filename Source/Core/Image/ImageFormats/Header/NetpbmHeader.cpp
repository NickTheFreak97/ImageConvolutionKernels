#include "NetpbmHeader.h"
#include <fstream>
#include <iostream>
#include <optional>

#include "../../../Utils/FileUtils.h"

NetpbmHeader::NetpbmHeader(
    ImageNetpbmFormat format,
    unsigned int rows,
    unsigned int columns,
    unsigned int max,
    const std::streampos &positionOfFirstPixel
) : format(format), rows(rows),  columns(columns), maxPixelValue(max), positionOfFirstPixel(positionOfFirstPixel) {

}

NetpbmHeader *NetpbmHeader::parsing(const std::filesystem::path &filePath) {
    std::ifstream fileHandler(filePath.string().c_str(), std::ios::binary);

    std::cout << filePath.string() << std::endl;
    if (!fileHandler.is_open()) {
        return nullptr;
    }

    std::string formatIdentifier;
    auto formatIdFromFile = FileUtils::getNextWord(fileHandler);

    if (!formatIdFromFile.has_value()) {
        return nullptr;
    } else {
        if (formatIdFromFile != "P3" && formatIdFromFile != "P6") {
            return nullptr;
        }
    }

    ImageNetpbmFormat format = (formatIdFromFile == "P3" ? ImageNetpbmFormat::PPM_ASCII : ImageNetpbmFormat::PPM_BINARY);

    auto widthIdentifier = FileUtils::getNextWord(fileHandler);
    auto heightIdentifier = FileUtils::getNextWord(fileHandler);

    if (!widthIdentifier.has_value() || !heightIdentifier.has_value()) {
        return nullptr;
    }

    auto width = std::stol(widthIdentifier.value());
    auto height = std::stol(heightIdentifier.value());

    if (width <= 0 || height <= 0) {
        return nullptr;
    }

    auto maxChannelValueToken = FileUtils::getNextWord(fileHandler);
    if (!maxChannelValueToken.has_value()) {
        return nullptr;
    }

    auto maxChannelValue = std::stol(maxChannelValueToken.value());
    if (maxChannelValue <= 0) {
        return nullptr;
    }

    char nextCharacter;
    while (fileHandler.get(nextCharacter)) {
        if (!std::isspace(nextCharacter)) {
            fileHandler.unget();
            break;
        }
    }

    auto firstPixelPosition = fileHandler.tellg();

    fileHandler.close();

    return new NetpbmHeader(format, height, width, maxChannelValue, firstPixelPosition);
}


ImageNetpbmFormat NetpbmHeader::getFormat() const {
    return this->format;
}

unsigned int NetpbmHeader::getRows() const {
    return this->rows;
}

unsigned int NetpbmHeader::getColumns() const {
    return this->columns;
}

unsigned int NetpbmHeader::getMaxPixelValue() const {
    return this->maxPixelValue;
}

std::streampos NetpbmHeader::getPositionOfFirstPixel() const {
    return this->positionOfFirstPixel;
}


