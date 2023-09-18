#include "parser/Parser.hpp"
#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include "parser/AST.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

Parser::Parser(std::unique_ptr<Lexer> l) : lexer(std::move(l)) {}

// Ver un token más adelante del actual.
Token Parser::getNextToken() {
  curTok = lexer->getToken();
  return curTok;
}

// manejo de errores (¡toca mejorar más adelante!)
std::unique_ptr<ExprAST> Parser::logError(const char *str) {
  std::cerr << "Error: " << str << "\n" << std::endl;
  return nullptr;
}

std::unique_ptr<PrototypeAST> Parser::logErrorP(const char *str) {
  logError(str);
  return nullptr;
}

// ----------------------------- //
// parsing básico de expresiones //
// ----------------------------- //

// numberExptr ::= number
// crea un nodo NumberExprAST, avanza el lexer al siguiente token
// retorna.
std::unique_ptr<ExprAST> Parser::parseNumberExpr() {
  auto result = std::make_unique<NumberExprAST>(
      lexer->getValNum()); // auto es para deducir el tipo de la variable
  getNextToken();
  return std::move(result);
}

// parenExpr ::= '(' expression ')'
std::unique_ptr<ExprAST> Parser::parseParenExpr() {
  getNextToken(); // se come '('
  auto v = parseExpression();
  if (!v) {
    return nullptr;
  }

  if (curTok.value != ")") {
    return logError("expected ')'");
  }

  getNextToken(); // se come ')'
  return v;
}

// identifierExpr
//   ::= identifier
//   ::= identifier '(' expression* ')'
std::unique_ptr<ExprAST> Parser::parseIdentifierExpr() {
  std::string idName = lexer->getIdentifierStr();

  getNextToken();

  // referencia a variable simple
  if (curTok.value != "(") {
    return std::make_unique<VariableExprAST>(idName);
  }

  // llamadas
  getNextToken(); // se come '('
  std::vector<std::unique_ptr<ExprAST>> args;
  while (curTok.value != ")") {
    if (auto arg =
            parseExpression()) { // basicamente, si logra parsear una expresión,
                                 // se considera como argumento de la función
      args.push_back(std::move(arg));
    } else {
      return nullptr;
    }

    if (curTok.value != ",") { // si parsea una expresión y no le sigue una coma
      return logError("expected ')' or ',' in argument list");
    }
    getNextToken();
  }

  getNextToken(); // se come ')'

  return std::make_unique<CallExprAST>(idName, std::move(args));
}

// primary
//   ::= identifierExpr
//   ::= numberExpr
//   ::= parenExpr
std::unique_ptr<ExprAST> Parser::parserPrimary() {
  switch (curTok.type) {
  default:
    return logError("unknown token when expecting an expression");
  case TokenType::TOK_ID:
    return parseIdentifierExpr();
  case TokenType::TOK_NUMBER:
    return parseNumberExpr();
  case TokenType::TOK_L_PAR:
    return parseParenExpr();
  }
}

// expression
//   ::= primary
std::unique_ptr<ExprAST> parseExpression() {
  // TO-DO
}