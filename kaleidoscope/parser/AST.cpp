#include "../codegen/Codegen.hpp"
#include "AST.hpp"
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Verifier.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

kaleidoscope::ExprAST::ExprAST(kaleidoscope::Codegen& codegen) : cn(codegen) {}

kaleidoscope::NumberExprAST::NumberExprAST(double val, kaleidoscope::Codegen& codegen)
    : ExprAST(codegen), val(val) {}

kaleidoscope::VariableExprAST::VariableExprAST(const std::string &name, kaleidoscope::Codegen& codegen)
    : ExprAST(codegen), name(name) {}

kaleidoscope::BinaryExprAST::BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS,
                             std::unique_ptr<ExprAST> RHS, kaleidoscope::Codegen& codegen)
    : ExprAST(codegen), op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

kaleidoscope::CallExprAST::CallExprAST(const std::string &callee,
                         std::vector<std::unique_ptr<ExprAST>> args, kaleidoscope::Codegen& codegen)
    : ExprAST(codegen), callee(callee), args(std::move(args)) {}

kaleidoscope::PrototypeAST::PrototypeAST(const std::string &name,
                           std::vector<std::string> args,
                           kaleidoscope::Codegen& codegen)
    : cn(codegen), name(name), args(std::move(args)) {}

std::string kaleidoscope::PrototypeAST::getName() { return name; }

kaleidoscope::FunctionAST::FunctionAST(std::unique_ptr<PrototypeAST> proto,
                         std::unique_ptr<ExprAST> body, kaleidoscope::Codegen& codegen)
    : cn(codegen), proto(std::move(proto)), body(std::move(body)) {}

// =============== //
// Code Generation //
// =============== //

// static std::unique_ptr<kaleidoscope::Codegen> cn;

llvm::Value *kaleidoscope::NumberExprAST::codegen() {
  // en LLVM IR, constantes numéricas se representan con ConstantFP
  return ConstantFP::get(*cn.TheContext, llvm::APFloat(val));
}

llvm::Value *kaleidoscope::VariableExprAST::codegen() {
  // buscar esta variable en la tabla de símbolos global
  Value *v = cn.namedValues[name];
  if (!v) {
    cn.logErrorV("Unknown variable name");
  }

  return v;
}

llvm::Value *kaleidoscope::BinaryExprAST::codegen() {
  Value *l = LHS->codegen();
  Value *r = RHS->codegen();
  if (!l || !r) {
    return nullptr;
  }

  switch (op) {
  case '+':
    return cn.Builder->CreateFAdd(l, r, "addtmp");
  case '-':
    return cn.Builder->CreateFSub(l, r, "subtmp");
  case '*':
    return cn.Builder->CreateFMul(l, r, "multmp");
  case '/':
    return cn.Builder->CreateFDiv(l, r, "divtmp");
  case '<':
    l = cn.Builder->CreateFCmpULT(l, r, "cmptmp");
    return cn.Builder->CreateUIToFP(
        l, Type::getDoubleTy(*cn.TheContext), "booltmp");
  case '>':
    l = cn.Builder->CreateFCmpUGT(l, r, "cmptmp");
    return cn.Builder->CreateUIToFP(
        l, Type::getDoubleTy(*cn.TheContext), "booltmp");
  default:
    return cn.logErrorV("invalid binary operator");
  }
}

llvm::Value *kaleidoscope::CallExprAST::codegen() {
  // busca el nombre de la función en la tabla de símbolos global
  Function *calleeF = cn.TheModule->getFunction(callee);
  if (!calleeF) {
    return cn.logErrorV("unknwown function referenced");
  }

  // compara tamaño de lista de argumentos
  if (calleeF->arg_size() != args.size()) {
    return cn.logErrorV("incorrect number of arguments in function call");
  }

  // toca tener, además, una secicón de comparación de tipo de dato de cada
  // parámetro

  std::vector<Value *> argsV;
  for (unsigned i = 0, e = args.size(); i != e; ++i) {
    argsV.push_back(args[i]->codegen());
    if (!argsV.back()) {
      return nullptr;
    }
  }

  return cn.Builder->CreateCall(calleeF, argsV, "calltmp");
}

llvm::Function *kaleidoscope::PrototypeAST::codegen() {
  // hace el tipo de la función -> crea un vector de "N" tipos double LLVM
  std::vector<Type *> doubles(args.size(),
                              Type::getDoubleTy(*cn.TheContext));

  // crea una función que tiene como parámetro "N" doubles y retorna uno
  FunctionType *ft = FunctionType::get(Type::getDoubleTy(*cn.TheContext),
                                       doubles, false);

  // se genera la función LLVM correspondiente
  Function *f = Function::Create(
      ft, Function::ExternalLinkage, // se refiere a que la función puede o no
                                     // ser definida en el módulo actual
      name,
      cn.TheModule.get()); // registra el nombre nuevo

  // nombres para los parámetros
  unsigned idx = 0;
  for (auto &arg : f->args()) {
    arg.setName(args[idx++]);
  }

  return f;
}

llvm::Function *kaleidoscope::FunctionAST::codegen() {
  // revisa si hay una función existente de alguna otra declaración
  Function *TheFunction = cn.TheModule->getFunction(proto->getName());

  // asegurarse que no tenga cuerpo aún
  if (!TheFunction) {
    TheFunction = proto->codegen();
  }

  if (!TheFunction) {
    return nullptr;
  }

  if (!TheFunction->empty()) {
    return (Function *)cn.logErrorV("function cannot be redefined.");
  }

  // crear un nuevo bloque básico para empezar la inserción
  //  -> la idea es ir "insertando" instrucciones a la función
  BasicBlock *BB =
      BasicBlock::Create(*cn.TheContext, "entry", TheFunction);
  cn.Builder->SetInsertPoint(BB);

  // guardar los parámetros de la función en el mapa namedValues
  cn.namedValues.clear();
  for (auto &arg : TheFunction->args()) {
    cn.namedValues[std::string(arg.getName())] = &arg;
  }

  if (Value *retVal = body->codegen()) {
    // crea retorno de función (fin de función)
    cn.Builder->CreateRet(retVal);

    // validar el código generado, revisar consistencia
    if (verifyFunction(*TheFunction, &errs())) {
      TheFunction->eraseFromParent();
      return nullptr;
    }

    // optimización de función
    cn.TheFPM->run(*TheFunction);

    return TheFunction;
  }

  // error en el cuerpo, elimina la función
  TheFunction->eraseFromParent();
  return nullptr;
}
