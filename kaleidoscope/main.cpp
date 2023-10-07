#include <filesystem>
#include <iostream>
#include <memory>

#include "lexer/Lexer.hpp"
#include "reader/ConsoleReader.hpp"

int main() {
  kaleidoscope::Lexer lexer{std::make_unique<kaleidoscope::ConsoleReader>()};

  auto run{true};
  while (run) {
    auto token{lexer.nextToken()};
    std::cout << "Token {TYPE: " << kaleidoscope::to_string(token.type)
              << ", VALUE: \"" << token.value << "\", LITERAL_VALUE: "
              << (token.literalValue.has_value()
                      ? std::to_string(token.literalValue.value())
                      : "NONE")
              << "}\n";
    if (token.type == kaleidoscope::Token::TokenType::END_OF_FILE) {
      run = false;
    }
  }

  return 0;
}
