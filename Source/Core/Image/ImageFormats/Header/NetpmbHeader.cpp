#include "NetpmbHeader.h"
#include <fstream>
#include <optional>

NetpmbHeader::NetpmbHeader(ImageNetpbmFormat format, unsigned int rows, unsigned int columns, const std::streampos &positionOfFirstPixel) : format(format), rows(rows),  columns(columns), positionOfFirstPixel(positionOfFirstPixel) {

}

/*
 * This method extracts the next word out of a file handler. Comment lines, starting with #, are ignored, and trailing whitespaces are not included in the return value.
 * After the execution of this method, the cursor of the file points at the first whitespace after the returned word, if it exists, EOF otherwise.
 *
 * A null optional is returned if the intput file was not open or an empty word was extracted.
 */
std::optional<std::string> NetpmbHeader::getNextWord(std::ifstream& fromFile) {
    if (!fromFile.is_open()) {
        return std::nullopt;
    }

    std::string nextToken;
    char nextCharacter;

    while (fromFile.get(nextCharacter)) {
        if (nextCharacter == '#') {
            std::string commentLine;
            std::getline(fromFile, commentLine);
        } else {
            if (!std::isspace(nextCharacter)) {
                nextToken += nextCharacter;
                break;
            }
        }
    }

    while (fromFile.get(nextCharacter)) {
        if (std::isspace(nextCharacter)) {
            fromFile.unget();
            break;
        }

        nextToken += nextCharacter;
    }

    return nextToken.empty() ? std::nullopt : std::optional(nextToken);
}


NetpmbHeader *NetpmbHeader::parsing(const std::filesystem::path &filePath) {
    auto filename = filePath.filename().string();

    std::ifstream fileHandler(filename.c_str(), std::ios::binary);

    if (!fileHandler.is_open()) {
        return nullptr;
    }

    std::string formatIdentifier;
    auto formatIdFromFile = NetpmbHeader::getNextWord(fileHandler);

    if (!formatIdFromFile.has_value()) {
        return nullptr;
    } else {
        if (formatIdFromFile != "P3" && formatIdFromFile != "P6") {
            return nullptr;
        }
    }

    ImageNetpbmFormat format = (formatIdFromFile == "P3" ? ImageNetpbmFormat::PPM_ASCII : ImageNetpbmFormat::PPM_BINARY);

    auto widthIdentifier = NetpmbHeader::getNextWord(fileHandler);
    auto heightIdentifier = NetpmbHeader::getNextWord(fileHandler);

    if (!widthIdentifier.has_value() || !heightIdentifier.has_value()) {
        return nullptr;
    }

    auto width = std::stol(widthIdentifier.value());
    auto height = std::stol(heightIdentifier.value());

    if (width <= 0 || height <= 0) {
        return nullptr;
    }

    auto maxChannelValueToken = NetpmbHeader::getNextWord(fileHandler);
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

    return new NetpmbHeader(format, height, width, firstPixelPosition);
}


ImageNetpbmFormat NetpmbHeader::getFormat() const {
    return this->format;
}

unsigned int NetpmbHeader::getRows() const {
    return this->rows;
}

unsigned int NetpmbHeader::getColumns() const {
    return this->columns;
}

std::streampos NetpmbHeader::getPositionOfFirstPixel() const {
    return this->positionOfFirstPixel;
}


