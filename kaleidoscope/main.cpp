#include <filesystem>
#include <iostream>
#include <memory>

#include "lexer/Lexer.hpp"
#include "parser/Precedence.hpp"
#include "parser/Parser.hpp"
#include "reader/ConsoleReader.hpp"
#include "reader/FileReader.hpp"
#include "executor/Executor.hpp"

int main() {

  // ================================ //
  // PRUEBA INDIVIDUAL READER - LEXER //
  // ================================ //

  // kaleidoscope::Lexer lexer{std::make_unique<kaleidoscope::ConsoleReader>()};
  // kaleidoscope::Lexer lexer{std::make_unique<kaleidoscope::FileReader>("./kaleidoscope/examples/test_kaleidoscope.txt")};

  // auto run{true};
  // while (run) {
  //   auto token{lexer.nextToken()};
  //   std::cout << "Token {TYPE: " << kaleidoscope::to_string(token.type)
  //             << ", VALUE: \"" << token.value << "\", LITERAL_VALUE: "
  //             << (token.literalValue.has_value()
  //                     ? std::to_string(token.literalValue.value())
  //                     : "NONE")
  //             << "}\n";
  //   if (token.type == kaleidoscope::Token::TokenType::END_OF_FILE) {
  //     run = false;
  //   }
  // }

  // ======================== //
  // PRUEBA INDIVIDUAL PARSER //
  // ======================== //

  // kaleidoscope::Lexer lexer(
  //     std::make_unique<kaleidoscope::FileReader>("./kaleidoscope/examples/test_kaleidoscope.txt"));

  // kaleidoscope::Precedence precedence(0);
  // kaleidoscope::Parser parser(std::make_unique<kaleidoscope::Lexer>(std::make_unique<kaleidoscope::FileReader>(
  //                   "./kaleidoscope/examples/test_kaleidoscope.txt")),
  //               std::make_unique<kaleidoscope::Precedence>(precedence));

  // kaleidoscope::Token tok({kaleidoscope::Token::TokenType::INIT, ""});
  // parser.getNextToken();
  // while (tok.type != kaleidoscope::Token::TokenType::END_OF_FILE) {
  //   tok = lexer.nextToken();
  //   parser.parse();
  // }

  // std::cout << "Success!" << std::endl;

  // ===================== //
  // PRUEBA TOTAL EXECUTOR //
  // ===================== //

  kaleidoscope::Executor executor;
  executor.initializeMembers("./kaleidoscope/examples/test_kaleidoscope.txt"); //
  executor.initializeModuleAndPassManager();
  executor.mainLoop();


  return 0;
}
