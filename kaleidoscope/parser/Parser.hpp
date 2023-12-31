#pragma once

#include "../codegen/Codegen.hpp"
#include "../lexer/Lexer.hpp"
#include "../lexer/Token.hpp"
#include "../parser/AST.hpp"
#include "../parser/Precedence.hpp"
#include "../reader/Reader.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

namespace kaleidoscope {

class Parser {
public:
  Parser(std::unique_ptr<Lexer> lexer, std::unique_ptr<Precedence> precedence,
         std::unique_ptr<Codegen> cn);
  ~Parser() = default;
  Token getNextToken();
  Token getCurTok();
  std::unique_ptr<ExprAST> logError(const char *str);
  std::unique_ptr<PrototypeAST> logErrorP(const char *str);
  std::unique_ptr<ExprAST> parseNumberExpr();
  std::unique_ptr<ExprAST> parseParenExpr();
  std::unique_ptr<ExprAST> parseIdentifierExpr();
  std::unique_ptr<ExprAST> parsePrimary();
  std::unique_ptr<ExprAST> parseExpression();
  std::unique_ptr<ExprAST> parseBinOpRHS(int exprPrec,
                                         std::unique_ptr<ExprAST> LHS);
  std::unique_ptr<PrototypeAST> parsePrototype();
  std::unique_ptr<FunctionAST> parseDefinition();
  std::unique_ptr<PrototypeAST> parseExtern();
  std::unique_ptr<FunctionAST> parseTopLevelExpr();
  void printToken(const kaleidoscope::Token &token);
  void parse();

private:
  std::unique_ptr<Lexer> lexer;
  std::unique_ptr<Precedence> precedence;
  std::unique_ptr<Codegen> codegen;
  Token curTok;
};

} // namespace kaleidoscope
