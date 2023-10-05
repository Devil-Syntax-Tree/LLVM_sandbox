#include "codegen/Codegen.hpp"
#include "parser/Parser.hpp"
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
#include "codegen/KaleidoscopeJIT.hpp"

using namespace llvm;

// Inicialización de miembros estáticos públicos
std::unique_ptr<LLVMContext> Codegen::TheContext =
    std::make_unique<LLVMContext>();

std::unique_ptr<IRBuilder<>> Codegen::Builder =
    std::make_unique<IRBuilder<>>(*TheContext);

std::unique_ptr<Module> Codegen::TheModule =
    std::make_unique<Module>("root", *TheContext);

std::map<std::string, Value *> Codegen::namedValues;

std::unique_ptr<legacy::FunctionPassManager> Codegen::TheFPM =
    std::make_unique<legacy::FunctionPassManager>(TheModule.get());

// Inicialización de miembros estáticos privados
std::unique_ptr<Reader> Codegen::reader;
std::unique_ptr<Lexer> Codegen::lexer;
std::unique_ptr<Precedence> Codegen::precedence;
std::unique_ptr<Parser> Codegen::parser;

void Codegen::initializeStaticMembers() {
  reader = std::make_unique<Reader>("kaleidoscope/kaleidoscope.txt");
  lexer =
      std::make_unique<Lexer>(std::move(reader)); // Mover reader a lexer aquí
  precedence = std::make_unique<Precedence>(0);
  parser = std::make_unique<Parser>(
      std::move(lexer),
      std::move(precedence)); // Mover lexer y precedence a parser aquí
}

Value *Codegen::logErrorV(const char *str) {
  fprintf(stderr, "Error: %s\n", str);
  exit(EXIT_FAILURE);
  return nullptr;
}

void Codegen::initializeModuleAndPassManager() {
  //TheModule->setDataLayout(TheJIT->getDataLayout());

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

  parser->getNextToken();
}

void Codegen::handleDefinition() {
  if (auto fnAST = parser->parseDefinition()) {
    if (auto *fnIR = fnAST->codegen()) {
      fprintf(stderr, "- read function definition -\n");
      fnIR->print(errs());
      fprintf(stderr, "\n");
    }
  } else {
    // se salta token para recuperarse del error
    parser->getNextToken();
  }
}

void Codegen::handleExtern() {
  if (auto protoAST = parser->parseExtern()) {
    if (auto *fnIR = protoAST->codegen()) {
      fprintf(stderr, "- read function definition -\n");
      fnIR->print(errs());
      fprintf(stderr, "\n");
    }
  } else {
    // se salta token para recuperarse del error
    parser->getNextToken();
  }
}

void Codegen::handleTopLevelExpression() {
  // crea funciones anónimas
  if (auto fnAST = parser->parseTopLevelExpr()) {
    if (auto *fnIR = fnAST->codegen()) {
      fprintf(stderr, "- read top-level expression -\n");
      fnIR->print(errs());
      fprintf(stderr, "\n");

      // la elimina
      fnIR->eraseFromParent();
    }
  } else {
    parser->getNextToken();
  }
}

void Codegen::mainLoop() {
  while (parser->getCurTok().type != TokenType::TOK_EOF) {
    switch (parser->getCurTok().type) {
    case TokenType::TOK_EOF:
      std::cout << "Success!" << std::endl;
      exit(EXIT_SUCCESS);
      return;
    case TokenType::TOK_DEF:
      handleDefinition();
      break;
    case TokenType::TOK_EXTERN:
      handleExtern();
      break;
    default: // no hay ifs ni fors ni nada de eso
      handleTopLevelExpression();
      break;
    }
  }
}