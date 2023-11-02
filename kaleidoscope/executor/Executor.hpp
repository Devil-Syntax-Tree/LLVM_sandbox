#pragma once

#include "../reader/Reader.hpp"
#include "../lexer/Lexer.hpp"
#include "../parser/Precedence.hpp"
#include "../parser/Parser.hpp"
#include "../codegen/Codegen.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

namespace kaleidoscope {

class Executor {
public:
  void initializeMembers(const std::string& file_root);
  void initializeModuleAndPassManager();
  void handleDefinition();
  void handleExtern();
  void handleTopLevelExpression();
  void mainLoop();

private:
  std::unique_ptr<Reader> reader;
  std::unique_ptr<Lexer> lexer;
  std::unique_ptr<Precedence> precedence;
  std::unique_ptr<Parser> parser;
  std::unique_ptr<Codegen> codegen;
};

} // namespace kaleidoscope
