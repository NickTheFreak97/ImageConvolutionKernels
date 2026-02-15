#include "FileUtils.h"
#include <fstream>
#include <optional>


/*
 * This method extracts the next word out of a file handler. Comment lines, starting with #, are ignored, and trailing whitespaces are not included in the return value.
 * After the execution of this method, the cursor of the file points at the first whitespace after the returned word, if it exists, EOF otherwise.
 *
 * A null optional is returned if the intput file was not open or an empty word was extracted.
 */
std::optional<std::string> FileUtils::getNextWord(std::ifstream& fromFile) {
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

std::optional<std::string> FileUtils::getNextBytes(std::ifstream& fromFile, unsigned int requestedBytes) {
    if (!fromFile.is_open()) {
        return std::nullopt;
    }

    std::optional<std::string> nextToken;
    unsigned char nextCharacter;
    uint64_t nextPixelIntValue = 0;

    for (int i = 0; i < requestedBytes; i++) {
        fromFile.read(reinterpret_cast<char*>(&nextCharacter), 1);
        if (fromFile.good()) {
            nextPixelIntValue = (nextPixelIntValue << 8) | nextCharacter;
        } else {
            break;
        }
    }

    if (fromFile.good()) {
        nextToken = std::optional(std::to_string(nextPixelIntValue));
    } else {
        nextToken = std::nullopt;
    }

    return nextToken->empty() ? std::nullopt : std::optional(nextToken);
}

