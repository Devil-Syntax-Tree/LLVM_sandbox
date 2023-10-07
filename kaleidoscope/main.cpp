#include <filesystem>
#include <iostream>
#include <memory>

#include "lexer/Lexer.hpp"
#include "reader/ConsoleReader.hpp"
#include "codegen/Codegen.hpp"

int main() {
  // kaleidoscope::Lexer lexer{std::make_unique<kaleidoscope::ConsoleReader>()};

  // while (true) {
  //  auto token{lexer.nextToken()};
  //   std::cout << "Token {TYPE: " << kaleidoscope::to_string(token.type)
  //             << ", VALUE: \"" << token.value << "\", LITERAL_VALUE: "
  //             << (token.literalValue.has_value()
  //                     ? std::to_string(token.literalValue.value())
  //                     : "NONE")
  //             << "}\n";
  // }

  kaleidoscope::Codegen codegen;
  codegen.initializeStaticMembers();
  codegen.initializeModuleAndPassManager();
  codegen.mainLoop();

  return 0;
}
