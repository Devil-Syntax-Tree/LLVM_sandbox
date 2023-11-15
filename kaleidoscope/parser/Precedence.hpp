#pragma once

#include "../lexer/Token.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>

namespace kaleidoscope {
class Precedence {
public:
  Precedence(int n);
  ~Precedence() = default;
  int getTokenPrecedence(Token curTok);

private:
  std::map<char, int> precedenceMap;
  int num;
};

} // namespace kaleidoscope
