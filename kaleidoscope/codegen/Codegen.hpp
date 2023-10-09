#pragma once

#include "../lexer/Lexer.hpp"
#include "../reader/Reader.hpp"
#include "../lexer/Token.hpp"
#include "../parser/Parser.hpp"
#include "../parser/Precedence.hpp"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LegacyPassManager.h>

namespace kaleidoscope {

using namespace llvm;

class Codegen {
public:
  Codegen() = default;
  ~Codegen() = default;
  void initializeStaticMembers();
  Value *logErrorV(const char *str);
  void initializeModuleAndPassManager();
  void handleDefinition();
  void handleExtern();
  void handleTopLevelExpression();
  void mainLoop();
  static std::unique_ptr<LLVMContext>
      TheContext; // estructuras de datos como "tipos" y "constantes".
  static std::unique_ptr<IRBuilder<>>
      Builder; // ayuda a generar instrucciones LLVM.
  static std::unique_ptr<Module>
      TheModule; // construct de LLVM que contiene funciones y variables
                 // globales -> LA estructura top-level que usa LLVm IR.
  static std::map<std::string, Value *>
      namedValues; // mapa para llevar un registro de los valores que se han
                   // definido en el scope actual y su representación LLVM ->
                   // básicamente, una tabla de símbolos.
  static std::unique_ptr<legacy::FunctionPassManager>
      TheFPM; // para JIT.

private:
  static std::unique_ptr<Reader> reader;
  static std::unique_ptr<Lexer> lexer;
  static std::unique_ptr<Precedence> precedence;
  static std::unique_ptr<Parser> parser;
};

} // namespace kaleidoscope