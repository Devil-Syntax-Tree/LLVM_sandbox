#pragma once

#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include "parser/AST.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

// clase base para todo nodo de expresión
class Parser {
public:
  Parser(std::unique_ptr<Lexer> lexer);
  ~Parser() = default;
  Token getNextToken();
  std::unique_ptr<ExprAST> logError(const char *str);
  std::unique_ptr<PrototypeAST> logErrorP(const char *str);
  std::unique_ptr<ExprAST> parseNumberExpr();
  std::unique_ptr<ExprAST> parseParenExpr();
  std::unique_ptr<ExprAST> parseIdentifierExpr();
  std::unique_ptr<ExprAST> parserPrimary();

private:
  std::unique_ptr<Lexer> lexer;
  Token curTok;
};