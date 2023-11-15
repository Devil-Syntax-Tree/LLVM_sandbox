#include "../codegen/Codegen.hpp"
#include "../codegen/KaleidoscopeJIT.hpp"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

void kaleidoscope::Codegen::initializeMembers() {
  TheContext = std::make_unique<LLVMContext>();
  Builder = std::make_unique<IRBuilder<>>(*TheContext);
  TheModule = std::make_unique<Module>("root", *TheContext);
  TheFPM = std::make_unique<legacy::FunctionPassManager>(TheModule.get());
}

Value *kaleidoscope::Codegen::logErrorV(const char *str) {
  fprintf(stderr, "Error: %s\n", str);
  exit(EXIT_FAILURE);
  return nullptr;
}

void kaleidoscope::Codegen::initializeModuleAndPassManager() {
  initializeMembers();
  // TheModule->setDataLayout(TheJIT->getDataLayout());

  // passes de optimización:

  // hacer una optimización "peephole" simple y "bit-twiddling optzns".
  TheFPM->add(createInstructionCombiningPass());

  // reasociar expresiones
  TheFPM->add(createReassociatePass());

  // eliminar subexpresiones comunes
  TheFPM->add(createGVNPass());

  // simplificar el grafo de "flow" -> eliminar bloques un reachable, etc.
  TheFPM->add(createCFGSimplificationPass());

  TheFPM->doInitialization();

  // parser->getNextToken();
}
