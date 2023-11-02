#include "../lexer/Lexer.hpp"
#include "../lexer/Token.hpp"
#include "../parser/Precedence.hpp"
#include "../parser/Parser.hpp"
#include "../parser/AST.hpp"
#include "../codegen/Codegen.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <bits/stdc++.h>

kaleidoscope::Parser::Parser(std::unique_ptr<Lexer> l, std::unique_ptr<Precedence> p, std::unique_ptr<Codegen> cn)
    : lexer{std::move(l)}, precedence{std::move(p)}, codegen{std::move(cn)} {}

// adelantar al siguiente token
kaleidoscope::Token kaleidoscope::Parser::getNextToken() {
  curTok = lexer->nextToken();
  // std::cout << "valor token: " << curTok.value << std::endl;
  return curTok;
}

kaleidoscope::Token kaleidoscope::Parser::getCurTok() {
  return curTok;
}

// manejo de errores (¡toca mejorar más adelante!)
std::unique_ptr<kaleidoscope::ExprAST> kaleidoscope::Parser::logError(const char *str) {
  //std::cerr << "Error: " << str << "\n" << std::endl;
  fprintf(stderr, "Error: %s\n", str);
  exit(EXIT_FAILURE);
  return nullptr;
}

std::unique_ptr<kaleidoscope::PrototypeAST> kaleidoscope::Parser::logErrorP(const char *str) {
  logError(str);
  return nullptr;
}

// ----------------------------- //
// parsing básico de expresiones //
// ----------------------------- //

// numberExptr ::= number
// crea un nodo NumberExprAST, avanza el lexer al siguiente token
// retorna.
std::unique_ptr<kaleidoscope::ExprAST> kaleidoscope::Parser::parseNumberExpr() {
  //auto valNum = getCurTok().literalValue;
  auto result = std::make_unique<NumberExprAST>(
      lexer->getValNum(),
      *codegen); // auto es para deducir el tipo de la variable
  // lexer->getValNum()
  getNextToken();
  return std::move(result);
}

// parenExpr ::= '(' expression ')'
std::unique_ptr<kaleidoscope::ExprAST> kaleidoscope::Parser::parseParenExpr() {
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
std::unique_ptr<kaleidoscope::ExprAST> kaleidoscope::Parser::parseIdentifierExpr() {
  // std::string idName = getCurTok().literalValue;
  std::string idName = lexer->getIdentifierStr();

  getNextToken();

  // referencia a variable simple
  if (curTok.value != "(") {
    return std::make_unique<VariableExprAST>(idName, *codegen);
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
  if (curTok.type == kaleidoscope::Token::TokenType::ID) {
    getNextToken();
  }

  return std::make_unique<CallExprAST>(idName, std::move(args), *codegen);
}

// primary
//   ::= identifierExpr
//   ::= numberExpr
//   ::= parenExpr
std::unique_ptr<kaleidoscope::ExprAST> kaleidoscope::Parser::parsePrimary() {
  switch (curTok.type) {
  default:
    return logError("unknown token when expecting an expression");
  case kaleidoscope::Token::TokenType::ID:
    return parseIdentifierExpr();
  case kaleidoscope::Token::TokenType::NUMBER:
    return parseNumberExpr();
  case kaleidoscope::Token::TokenType::LEFT_PARENTHESES:
    return parseParenExpr();
  }
}

// expression
//   ::= primary opers
std::unique_ptr<kaleidoscope::ExprAST> kaleidoscope::Parser::parseExpression() {
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
std::unique_ptr<kaleidoscope::ExprAST> kaleidoscope::Parser::parseBinOpRHS(int exprPrec,
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
        std::make_unique<BinaryExprAST>(binOp, std::move(LHS), std::move(RHS), *codegen);
  }
}

// prototype
//   ::= id '(' id* ')'
std::unique_ptr<kaleidoscope::PrototypeAST> kaleidoscope::Parser::parsePrototype() {
  if (curTok.type != kaleidoscope::Token::TokenType::ID) {
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
    if (curTok.type == kaleidoscope::Token::TokenType::ID) { // revisar cuando se pone como fib(x, y z) -> sin coma -> me imagino que lo acepta...
      argNames.push_back(lexer->getIdentifierStr());
      getNextToken(); // saltar posible coma
    }

    if (curTok.value == ")") {
      break;
    }

    if (curTok.value == ",") {
      getNextToken();
      if (curTok.type !=kaleidoscope::Token::TokenType::ID || entryTok.type != kaleidoscope::Token::TokenType::ID) {
        return logErrorP("expected argument, maybe you forgot...");
      }
    }

    if (curTok.value != ")" or curTok.value != ",") {
      break;
    }
  }

  if (curTok.value != ")") {
    return logErrorP("expected ')' in prototype");
  }

  getNextToken(); // se come ')'

  return std::make_unique<PrototypeAST>(fnName, std::move(argNames), *codegen);
}

// definition
//   ::= 'def' prototype expression
std::unique_ptr<kaleidoscope::FunctionAST> kaleidoscope::Parser::parseDefinition() {
  getNextToken(); // se come 'def'
  auto proto = parsePrototype();
  if (!proto) {
    return nullptr;
  }

  if (curTok.type == kaleidoscope::Token::TokenType::END_OF_FILE) {
    std::cout << "Success!" << std::endl;
    exit(EXIT_SUCCESS);
  }

  if (auto e = parseExpression()) {
    return std::make_unique<FunctionAST>(std::move(proto), std::move(e), *codegen);
  }

  return nullptr;
}

// external
//    ::= 'extern' prototype
std::unique_ptr<kaleidoscope::PrototypeAST> kaleidoscope::Parser::parseExtern() {
  getNextToken(); // se come 'extern'
  return parsePrototype();
}

// topLevelExpr
//    ::= expression
std::unique_ptr<kaleidoscope::FunctionAST> kaleidoscope::Parser::parseTopLevelExpr() {
  if (auto e = parseExpression()) {
    // proto anónimo
    auto proto = std::make_unique<PrototypeAST>("", std::vector<std::string>(), *codegen);
    return std::make_unique<FunctionAST>(std::move(proto), std::move(e), *codegen);
  }

  return nullptr;
}

void kaleidoscope::Parser::parse() {
  switch (curTok.type) {
    case kaleidoscope::Token::TokenType::END_OF_FILE:
      std::cout << "Success!" << std::endl;
      exit(EXIT_SUCCESS);
      return;
    case kaleidoscope::Token::TokenType::DEF:
      parseDefinition();
      break;
    default:
      parseExpression();
      break;
  }
}
