#include "parser/AST.hpp"
#include "codegen/Codegen.hpp"
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Verifier.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

NumberExprAST::NumberExprAST(double val) : val(val) {}

VariableExprAST::VariableExprAST(const std::string &name) : name(name) {}

BinaryExprAST::BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS,
                             std::unique_ptr<ExprAST> RHS)
    : op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

CallExprAST::CallExprAST(const std::string &callee,
                         std::vector<std::unique_ptr<ExprAST>> args)
    : callee(callee), args(std::move(args)) {}

PrototypeAST::PrototypeAST(const std::string &name,
                           std::vector<std::string> args)
    : name(name), args(std::move(args)) {}

std::string PrototypeAST::getName() { return name; }

FunctionAST::FunctionAST(std::unique_ptr<PrototypeAST> proto,
                         std::unique_ptr<ExprAST> body)
    : proto(std::move(proto)), body(std::move(body)) {}

// =============== //
// Code Generation //
// =============== //

static std::unique_ptr<Codegen> cn;

Value *NumberExprAST::codegen() {
  // en LLVM IR, constantes numéricas se representan con ConstantFP
  return ConstantFP::get(*Codegen::TheContext, llvm::APFloat(val));
}

Value *VariableExprAST::codegen() {
  // buscar esta variable en la tabla de símbolos global
  Value *v = Codegen::namedValues[name];
  if (!v) {
    cn->logErrorV("Unknown variable name");
  }

  return v;
}

Value *BinaryExprAST::codegen() {
  Value *l = LHS->codegen();
  Value *r = RHS->codegen();
  if (!l || !r) {
    return nullptr;
  }

  switch (op) {
  case '+':
    return Codegen::Builder->CreateFAdd(l, r, "addtmp");
  case '-':
    return Codegen::Builder->CreateFSub(l, r, "subtmp");
  case '*':
    return Codegen::Builder->CreateFMul(l, r, "multmp");
  case '/':
    return Codegen::Builder->CreateFDiv(l, r, "divtmp");
  case '<':
    l = Codegen::Builder->CreateFCmpULT(l, r, "cmptmp");
    return Codegen::Builder->CreateUIToFP(
        l, Type::getDoubleTy(*Codegen::TheContext), "booltmp");
  case '>':
    l = Codegen::Builder->CreateFCmpUGT(l, r, "cmptmp");
    return Codegen::Builder->CreateUIToFP(
        l, Type::getDoubleTy(*Codegen::TheContext), "booltmp");
  default:
    return cn->logErrorV("invalid binary operator");
  }
}

Value *CallExprAST::codegen() {
  // busca el nombre de la función en la tabla de símbolos global
  Function *calleeF = Codegen::TheModule->getFunction(callee);
  if (!calleeF) {
    return cn->logErrorV("unknwown function referenced");
  }

  // compara tamaño de lista de argumentos
  if (calleeF->arg_size() != args.size()) {
    return cn->logErrorV("incorrect number of arguments in function call");
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

  return Codegen::Builder->CreateCall(calleeF, argsV, "calltmp");
}

Function *PrototypeAST::codegen() {
  // hace el tipo de la función -> crea un vector de "N" tipos double LLVM
  std::vector<Type *> doubles(args.size(),
                              Type::getDoubleTy(*Codegen::TheContext));

  // crea una función que tiene como parámetro "N" doubles y retorna uno
  FunctionType *ft = FunctionType::get(Type::getDoubleTy(*Codegen::TheContext),
                                       doubles, false);

  // se genera la función LLVM correspondiente
  Function *f = Function::Create(
      ft, Function::ExternalLinkage, // se refiere a que la función puede o no
                                     // ser definida en el módulo actual
      name,
      Codegen::TheModule.get()); // registra el nombre nuevo

  // nombres para los parámetros
  unsigned idx = 0;
  for (auto &arg : f->args()) {
    arg.setName(args[idx++]);
  }

  return f;
}

Function *FunctionAST::codegen() {
  // revisa si hay una función existente de alguna otra declaración
  Function *TheFunction = Codegen::TheModule->getFunction(proto->getName());

  // asegurarse que no tenga cuerpo aún
  if (!TheFunction) {
    TheFunction = proto->codegen();
  }

  if (!TheFunction) {
    return nullptr;
  }

  if (!TheFunction->empty()) {
    return (Function *)cn->logErrorV("function cannot be redefined.");
  }

  // crear un nuevo bloque básico para empezar la inserción
  //  -> la idea es ir "insertando" instrucciones a la función
  BasicBlock *BB =
      BasicBlock::Create(*Codegen::TheContext, "entry", TheFunction);
  Codegen::Builder->SetInsertPoint(BB);

  // guardar los parámetros de la función en el mapa namedValues
  Codegen::namedValues.clear();
  for (auto &arg : TheFunction->args()) {
    Codegen::namedValues[std::string(arg.getName())] = &arg;
  }

  if (Value *retVal = body->codegen()) {
    // crea retorno de función (fin de función)
    Codegen::Builder->CreateRet(retVal);

    // validar el código generado, revisar consistencia
    if (verifyFunction(*TheFunction, &errs())) {
      TheFunction->eraseFromParent();
      return nullptr;
    }

    // optimización de función
    Codegen::TheFPM->run(*TheFunction);

    return TheFunction;
  }

  // error en el cuerpo, elimina la función
  TheFunction->eraseFromParent();
  return nullptr;
}