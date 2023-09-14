#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <lexer/Lexer.h>
#include <lexer/Token.h>
#include <lexer/Reader.h>

Lexer::Lexer(Reader &r) : reader(r) {}

Lexer::~Lexer() {}

int Lexer::getToken() {
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
      return TOK_DEF;
    }
    if (identifierStr == "extern") {
      return TOK_EXTERN;
    }
    if (identifierStr == "if") {
      return TOK_IF;
    }
    if (identifierStr == "then") {
      return TOK_THEN;
    }
    if (identifierStr == "else") {
      return TOK_ELSE;
    }
    
    return TOK_ID;
  }
  
  // reconocer números -> [0-9.]+
  if (isdigit(lastCharacter) || lastCharacter == '.') {
    std::string numStr;
    while (isdigit(reader.peekNextChar()) || reader.peekNextChar() == '.') {
      numStr += lastCharacter;
      lastCharacter = reader.readChar();
    }

    valNum = strtod(numStr.c_str(), 0);
    return TOK_NUMBER;
  }

  // reconocer símbolos
  if (lastCharacter == '=') {
    return TOK_ASSIGN;
  }
  if (lastCharacter == '+') {
    return TOK_SUM;
  }
  if (lastCharacter == '-') {
    return TOK_DIFF;
  }
  if (lastCharacter == '*') {
    return TOK_MULT;
  }
  if (lastCharacter == '/') {
    return TOK_DIV;
  }
  if (lastCharacter == '<') {
    return TOK_LESS;
  }
  if (lastCharacter == '>') {
    return TOK_GREATER;
  }
  if (lastCharacter == '(') {
    return TOK_L_PAR;
  }
  if (lastCharacter == ')') {
    return TOK_R_PAR;
  }

  // reconocer EOF
  if (lastCharacter == EOF) {
    return TOK_EOF;
  }

  // caso para símbolos extraños... realmente, esto no debería existir...
  // en todo caso, retorna su código ASCII
  int thisCharacter = lastCharacter;
  lastCharacter = reader.readChar();
  return thisCharacter;
}
