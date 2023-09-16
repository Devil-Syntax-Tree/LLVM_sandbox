#pragma once

#include <functional>

#include "Token.hpp"

class Lexer {
public:
  Lexer(std::function<char()> r) : reader{r} {}

  Token getNextToken() {
    /*
       callback for reader
       it allows for flexibility in the lexer
       it decouples how we obtain the characters from how we process them
       TODO: investigate how to read utf-8 characters
       could break API
       auto mitigates API breaking
       TODO: define error handling
    */
    lastCharacter = reader();
    while (lastCharacter == ' ') {
      lastCharacter = reader();
    }
  }

private:
  std::function<char()> reader;
  char lastCharacter;
};
