#pragma once

#include "parser/Parser.hpp"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

using namespace llvm;

class Codegen {
public:
    Codegen() = default;
    ~Codegen() = default;
    Value* LogErrorV(const char *str);
    static std::unique_ptr<LLVMContext> TheContext; // estructuras de datos como "tipos" y "constantes"
    static std::unique_ptr<IRBuilder<>> Builder; // ayuda a generar instrucciones LLVM
    static std::unique_ptr<Module> TheModule; // construct de LLVM que contiene funciones y variables globales -> LA estructura top-level que usa LLVm IR
    static std::map<std::string, Value *> namedValues; // mapa para llevar un registro de los valores que se han definido en el scope actual y su representación LLVM -> básicamente, una tabla de símbolos.

private:
    std::unique_ptr<Parser> parser;
};