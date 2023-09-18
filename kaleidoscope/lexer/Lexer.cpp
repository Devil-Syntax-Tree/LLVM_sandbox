#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include "lexer/Reader.hpp"

Lexer::Lexer(Reader &r) : reader(r) {}

Token Lexer::getToken() {
  int lastCharacter = ' ';

  while (isspace(lastCharacter)) { // ignora espacios
    lastCharacter = reader.readChar();
  }

  // reconocer identificadores -> [aA-zZ][aA-zZ0-9]*
  if (isalpha(lastCharacter)) { // isalpha() revisa si el primer carácter es letra (min o may)
    identifierStr = lastCharacter;
    while (isalnum(reader.peekNextChar())) { // isalnum() revisa que luego haya letras o números
      lastCharacter = reader.readChar();
      identifierStr += lastCharacter;
    }

    if (identifierStr == "def") {
      return { TokenType::TOK_DEF, identifierStr };
    }
    if (identifierStr == "extern") {
      return { TokenType::TOK_EXTERN, identifierStr };
    }
    if (identifierStr == "if") {
      return { TokenType::TOK_IF, identifierStr };
    }
    if (identifierStr == "then") {
      return { TokenType::TOK_THEN, identifierStr };
    }
    if (identifierStr == "else") {
      return { TokenType::TOK_ELSE, identifierStr };
    }
    
    return { TokenType::TOK_ID, identifierStr };
  }
  
  // reconocer números -> [0-9.]+
  if (isdigit(lastCharacter) || lastCharacter == '.') {
    std::string numStr;
    numStr += lastCharacter;
    while (isdigit(reader.peekNextChar()) || reader.peekNextChar() == '.') {
      lastCharacter = reader.readChar();
      numStr += lastCharacter;
    }

    valNum = strtod(numStr.c_str(), 0);
    return { TokenType::TOK_NUMBER, numStr };
  }

  // reconocer símbolos
  if (lastCharacter == '=') {
    return { TokenType::TOK_ASSIGN, std::string(1, lastCharacter) };
  }
  if (lastCharacter == '+') {
    return { TokenType::TOK_SUM, std::string(1, lastCharacter) };
  }
  if (lastCharacter == '-') {
    return { TokenType::TOK_DIFF, std::string(1, lastCharacter) };
  }
  if (lastCharacter == '*') {
    return { TokenType::TOK_MULT, std::string(1, lastCharacter) };
  }
  if (lastCharacter == '/') {
    return { TokenType::TOK_DIV, std::string(1, lastCharacter) };
  }
  if (lastCharacter == '<') {
    return { TokenType::TOK_LESS, std::string(1, lastCharacter) };
  }
  if (lastCharacter == '>') {
    return { TokenType::TOK_GREATER, std::string(1, lastCharacter) };
  }
  if (lastCharacter == '(') {
    return { TokenType::TOK_L_PAR, std::string(1, lastCharacter) };
  }
  if (lastCharacter == ')') {
    return { TokenType::TOK_R_PAR, std::string(1, lastCharacter) };
  }

  // reconocer EOF
  if (lastCharacter == EOF) {
    return { TokenType::TOK_EOF, "EOF" };
  }

  // caso para símbolos extraños... realmente, esto no debería existir...
  // en todo caso, retorna su código ASCII
  //int thisCharacter = lastCharacter;
  lastCharacter = reader.readChar();
  return { TokenType::TOK_UNKNOWN, std::string(1, lastCharacter) };
}