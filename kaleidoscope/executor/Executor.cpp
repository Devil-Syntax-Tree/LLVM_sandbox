#include "Executor.hpp"
#include "../codegen/Codegen.hpp"
#include "../lexer/Lexer.hpp"
#include "../parser/Parser.hpp"
#include "../parser/Precedence.hpp"
#include "../reader/ConsoleReader.hpp"
#include "../reader/FileReader.hpp"
#include "../reader/Reader.hpp"

void kaleidoscope::Executor::initializeMembers(const std::string &file_root) {
  // if (!file_root.empty()) {
  //     reader = std::make_unique<kaleidoscope::FileReader>(file_root);
  // } else {
  //     reader = std::make_unique<kaleidoscope::ConsoleReader>();
  // }
  reader = std::make_unique<kaleidoscope::FileReader>(file_root);
  lexer = std::make_unique<kaleidoscope::Lexer>(
      std::move(reader)); // Mover reader a lexer aquí
  precedence = std::make_unique<kaleidoscope::Precedence>(0);
  codegen = std::make_unique<kaleidoscope::Codegen>();
  codegen->initializeModuleAndPassManager();
  parser = std::make_unique<kaleidoscope::Parser>(
      std::move(lexer), std::move(precedence),
      std::move(codegen)); // Mover lexer, precedence y codegen a parser aquí
}

void kaleidoscope::Executor::initializeModuleAndPassManager() {
  parser->getNextToken();
}

void kaleidoscope::Executor::handleDefinition() {
  if (auto fnAST = parser->parseDefinition()) {
    if (auto *fnIR = fnAST->codegen()) {
      fprintf(stderr, "- reading function definition -\n");
      fnIR->print(errs());
      fprintf(stderr, "\n");
    }
  } else {
    // se salta token para recuperarse del error
    parser->getNextToken();
  }
}

void kaleidoscope::Executor::handleExtern() {
  if (auto protoAST = parser->parseExtern()) {
    if (auto *fnIR = protoAST->codegen()) {
      fprintf(stderr, "- reading function definition -\n");
      fnIR->print(errs());
      fprintf(stderr, "\n");
    }
  } else {
    // se salta token para recuperarse del error
    parser->getNextToken();
  }
}

void kaleidoscope::Executor::handleTopLevelExpression() {
  // crea funciones anónimas
  if (auto fnAST = parser->parseTopLevelExpr()) {
    if (auto *fnIR = fnAST->codegen()) {
      fprintf(stderr, "- reading top-level expression -\n");
      fnIR->print(errs());
      fprintf(stderr, "\n");

      // la elimina
      fnIR->eraseFromParent();
    }
  } else {
    parser->getNextToken();
  }
}

void kaleidoscope::Executor::mainLoop() {
  while (parser->getCurTok().type !=
         kaleidoscope::Token::TokenType::END_OF_FILE) {
    switch (parser->getCurTok().type) {
    case kaleidoscope::Token::TokenType::END_OF_FILE:
      std::cout << "Success!" << std::endl;
      exit(EXIT_SUCCESS);
      return;
    case kaleidoscope::Token::TokenType::DEF:
      handleDefinition();
      break;
    case kaleidoscope::Token::TokenType::EXTERN:
      handleExtern();
      break;
    default: // no hay ifs ni fors ni nada de eso
      handleTopLevelExpression();
      break;
    }
  }
}
