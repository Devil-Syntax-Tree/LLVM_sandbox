#include "parser/Parser.hpp"
#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include "parser/AST.hpp"
#include "parser/Precedence.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <bits/stdc++.h>

Parser::Parser(std::unique_ptr<Lexer> l, std::unique_ptr<Precedence> p)
    : lexer(std::move(l)), precedence(std::move(p)) {}

// Ver un token más adelante del actual.
Token Parser::getNextToken() {
  curTok = lexer->getToken();
  std::cout << "valor token: " << curTok.value << std::endl;
  return curTok;
}

// manejo de errores (¡toca mejorar más adelante!)
std::unique_ptr<ExprAST> Parser::logError(const char *str) {
  //std::cerr << "Error: " << str << "\n" << std::endl;
  fprintf(stderr, "Error: %s\n", str);
  exit(EXIT_FAILURE);
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
  if (curTok.value != ")") {
    while (true) {
      if (auto arg =
              parseExpression()) { // basicamente, si logra parsear una expresión,
                                  // se considera como argumento de la función
        args.push_back(std::move(arg));
      } else {
        return nullptr;
      }

      if (curTok.value == ")") {
        break;
      }

      if (curTok.value != ",") { // si parsea una expresión y no le sigue una coma
        return logError("expected ')' or ',' in argument list");
      }

      getNextToken();
    }
  }

  getNextToken(); // se come ')'
  if (curTok.type == TokenType::TOK_ID) {
    getNextToken();
  }

  return std::make_unique<CallExprAST>(idName, std::move(args));
}

// primary
//   ::= identifierExpr
//   ::= numberExpr
//   ::= parenExpr
std::unique_ptr<ExprAST> Parser::parsePrimary() {
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
//   ::= primary opers
std::unique_ptr<ExprAST> Parser::parseExpression() {
  auto LHS = parsePrimary();
  if (!LHS) {
    return nullptr;
  }

  return parseBinOpRHS(0, std::move(LHS));
}

// opers
//  ::= ('+''-''*'... primary)*
/**
 * Ejemplo de ejecución de la función:
 * ej. 3 + 4 * 5 -> + tiene menor precedencia que *
 * se comienza con 3 como LHS y 0 como precedencia
 * 3 >= 0, entonces sigue
 * se pasa 4 como LHS y * como precedencia
 * como la precedencia de * es mayor que la de +, se invoca la función, con 4 como LHS
 * se procesa 5 como RHS y * como precedencia
 * se combina 4 y 5 con operador * en el nodo BinaryExprAST
 * se combina el resultado anterior con 3 con operador + en un nodo BinaryExprAST
*/
std::unique_ptr<ExprAST> Parser::parseBinOpRHS(int exprPrec,
                                               std::unique_ptr<ExprAST> LHS) {

  while (true) { // sabemos que va a acabar cuando token inválidos son -1
                 // es decir, cuando no haya otro par [operador, expresión], acaba.
    
    // si es un operador, encontrar la precedencia
    int tokPrec = precedence->getTokenPrecedence(curTok);

    // compara la precedencia de uno y del otro
    if (tokPrec < exprPrec) {
      return LHS;
    }
    
    char arr[curTok.value.length() + 1];
    strcpy(arr, curTok.value.c_str());
    char binOp = arr[0];

    getNextToken(); // se come el operador

    // parsear la expresión que está después del operador
    auto RHS = parsePrimary();
    if (!RHS) {
      return nullptr;
    }

    // si la precedencia del operador a la derecha es mayor que
    // el operador actual, se evalúa con RHS como LHS
    int nextPrec = precedence->getTokenPrecedence(curTok);
    if (tokPrec < nextPrec) {
      RHS = parseBinOpRHS(tokPrec + 1, std::move(RHS));
      if (!RHS) {
        return nullptr;
      }
    }

    // juntar LHS y RHS
    LHS =
        std::make_unique<BinaryExprAST>(binOp, std::move(LHS), std::move(RHS));
  }
}

// prototype
//   ::= id '(' id* ')'
std::unique_ptr<PrototypeAST> Parser::parsePrototype() {
  if (curTok.type != TokenType::TOK_ID) {
    return logErrorP("expected function name in prototype");
  }

  // nombre de función
  std::string fnName = lexer->getIdentifierStr();
  getNextToken();

  // debe seguir un '('
  if (curTok.value != "(") {
    return logErrorP("expected '(' in prototype");
  }

  // leer lista de argumentos
  std::vector<std::string> argNames;
  getNextToken();
  Token entryTok = curTok;
  while (true) {
    if (curTok.type == TokenType::TOK_ID) { // revisar cuando se pone como fib(x, y z) -> sin coma -> me imagino que lo acepta...
      argNames.push_back(lexer->getIdentifierStr());
      getNextToken(); // saltar posible coma
    }

    if (curTok.value == ")") {
      break;
    }

    if (curTok.value == ",") {
      getNextToken();
      if (curTok.type != TokenType::TOK_ID || entryTok.type != TokenType::TOK_ID) {
        return logErrorP("expected argument, maybe you forgot...");
      }
    }
  }

  if (curTok.value != ")") {
    return logErrorP("expected ')' in prototype");
  }
  
  getNextToken(); // se come ')'

  return std::make_unique<PrototypeAST>(fnName, std::move(argNames));
}

// definition
//   ::= 'def' prototype expression
std::unique_ptr<FunctionAST> Parser::parseDefinition() {
  getNextToken(); // se come 'def'
  auto proto = parsePrototype();
  if (!proto) {
    return nullptr;
  }

  if (curTok.type == TokenType::TOK_EOF) {
    std::cout << "Success!" << std::endl;
    exit(EXIT_SUCCESS);
  }

  if (auto e = parseExpression()) {
    return std::make_unique<FunctionAST>(std::move(proto), std::move(e));
  }

  return nullptr;
}

// external
//    ::= 'extern' prototype
std::unique_ptr<PrototypeAST> Parser::parseExtern() {
  getNextToken(); // se come 'extern'
  return parsePrototype();
}

// topLevelExpr
//    ::= expression
std::unique_ptr<FunctionAST> Parser::parseTopLevelExpr() {
  if (auto e = parseExpression()) {
    // proto anónimo
    auto proto = std::make_unique<PrototypeAST>("", std::vector<std::string>());
    return std::make_unique<FunctionAST>(std::move(proto), std::move(e));
  }

  return nullptr;
}

void Parser::parse() {
  switch (curTok.type) {
    case TokenType::TOK_EOF:
      std::cout << "Success!" << std::endl;
      exit(EXIT_SUCCESS);
      return;
    case TokenType::TOK_DEF:
      parseDefinition();
      break;
    default:
      parseExpression();
      break;
  }
}
