#pragma once

#include "../lexer/Token.hpp"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <map>
namespace kaleidoscope {

using namespace llvm;

class Codegen {
public:
  Codegen() = default;
  ~Codegen() = default;
  void initializeMembers();
  Value *logErrorV(const char *str);
  void initializeModuleAndPassManager();

  std::unique_ptr<LLVMContext>
      TheContext; // estructuras de datos como "tipos" y "constantes".
  std::unique_ptr<IRBuilder<>> Builder; // ayuda a generar instrucciones LLVM.
  std::unique_ptr<Module>
      TheModule; // construct de LLVM que contiene funciones y variables
                 // globales -> LA estructura top-level que usa LLVm IR.
  std::map<std::string, Value *>
      namedValues; // mapa para llevar un registro de los valores que se han
                   // definido en el scope actual y su representación LLVM ->
                   // básicamente, una tabla de símbolos.
  std::unique_ptr<legacy::FunctionPassManager> TheFPM; // para JIT.
};

} // namespace kaleidoscope
