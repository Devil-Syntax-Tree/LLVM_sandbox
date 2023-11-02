#pragma once

namespace kaleidoscope {
class Reader {
public:
  virtual ~Reader() {}
  // TODO: refactor to add UNICODE
  virtual char nextCharacter() = 0;
};
} // namespace kaleidoscope

// #ifndef READER_HPP
// #define READER_HPP

// #include <string>
// #include <fstream>

// namespace kaleidoscope {

// class Reader {
// public:
//     Reader(); // Constructor para ConsoleReader
//     Reader(const std::string& path); // Constructor para FileReader
//     virtual ~Reader() = default;

//     char nextCharacter();

// private:
//     enum class Type {
//         CONSOLE,
//         FILE
//     };

//     Type readerType;
//     std::ifstream file;
// };

// } // namespace kaleidoscope

// #endif // READER_HPP
