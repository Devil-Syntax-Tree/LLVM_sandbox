#include "parser/AST.hpp"
#include "codegen/Codegen.hpp"
#include "llvm/ADT/APFloat.h"
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
    // buscar esta variable en la tabla de símbolos
    Value *v = Codegen::namedValues[name];
    if (!v) {
        cn->LogErrorV("Unknown variable name");
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
            return Codegen::Builder->CreateUIToFP(l, Type::getDoubleTy(*Codegen::TheContext), "booltmp");
        case '>':
            l = Codegen::Builder->CreateFCmpUGT(l, r, "cmptmp");
            return Codegen::Builder->CreateUIToFP(l, Type::getDoubleTy(*Codegen::TheContext), "booltmp");
    }
}