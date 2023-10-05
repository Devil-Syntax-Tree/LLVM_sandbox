#include "ConsoleReader.hpp"

#include <iostream>

char kaleidoscope::ConsoleReader::nextChar() {
    if (hasBufferedChar) {
        hasBufferedChar = false;
        return buffer;
    }
    return std::cin.get();
}

char kaleidoscope::ConsoleReader::peekNextChar() {
    if (!hasBufferedChar) {
        buffer = std::cin.get();
        hasBufferedChar = true;
    }
    return buffer;
}
