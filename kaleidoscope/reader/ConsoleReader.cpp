#include <iostream>

#include "ConsoleReader.hpp"

namespace kaleidoscope {
char ConsoleReader::nextCharacter() { return std::cin.get(); }
} // namespace kaleidoscope
