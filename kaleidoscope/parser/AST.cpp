#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "parser/AST.hpp"

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