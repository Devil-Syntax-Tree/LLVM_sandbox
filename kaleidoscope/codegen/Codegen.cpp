#include "codegen/Codegen.hpp"
#include "parser/Parser.hpp"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

using namespace llvm;

std::unique_ptr<LLVMContext> Codegen::TheContext =
    std::make_unique<LLVMContext>();
std::unique_ptr<IRBuilder<>> Codegen::Builder =
    std::make_unique<IRBuilder<>>(*TheContext);
std::unique_ptr<Module> Codegen::TheModule =
    std::make_unique<Module>("root", *TheContext);
std::map<std::string, Value *> Codegen::namedValues;

Value* Codegen::logErrorV(const char *str) {
  fprintf(stderr, "Error: %s\n", str);
  exit(EXIT_FAILURE);
  return nullptr;
}