#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <lexer/Lexer.h>
#include <lexer/Token.h>

Lexer::Lexer(std::function<char()> r) : reader(r) {
  //lastCharacter = reader(); // reader es leerChar() de la clase Reader
}

Lexer::~Lexer() {}

int Lexer::getToken() {
  int lastCharacter = ' ';

  while (lastCharacter == ' ') { // ignora espacios
    lastCharacter = reader();

  // reconocer identificadores -> [aA-zZ][aA-zZ0-9]*
  if (isalpha(lastCharacter)) { // isalpha() revisa si el primer carácter es letra (min o may)
    identifierStr = lastCharacter;
    while (isalnum((lastCharacter = reader()))) { // isalnum() revisa que luego haya letras o números
      identifierStr += lastCharacter;
    }

    return TOK_ID;
  }
  // reconocer números -> [0-9.]+
  if (isdigit(lastCharacter) || lastCharacter == '.') {
    std::string numStr;
    while (isdigit(lastCharacter) || lastCharacter == '.') {
      numStr += lastCharacter;
      lastCharacter = reader();
    }

    valNum = strtod(numStr.c_str(), 0);
    return TOK_NUMBER;
  }

  // reconocer EOF
  if (lastCharacter == EOF) {
    return TOK_EOF;
  }

  int thisCharacter = lastCharacter;
  lastCharacter = reader();
  return thisCharacter;
}
}