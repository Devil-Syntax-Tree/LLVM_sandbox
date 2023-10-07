#pragma once

#include <memory>
#include <optional>

#include "Token.hpp"

namespace kaleidoscope {
class Reader;
class Lexer {
public:
  Lexer(std::unique_ptr<Reader> reader);
  std::string getIdentifierStr();
  double getValNum();
  Token nextToken();

private:
  std::unique_ptr<Reader> reader;
  std::optional<char> pendingCharacter;
};

} // namespace kaleidoscope
