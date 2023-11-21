#pragma once

#include "../codegen/Codegen.hpp"
#include <fstream>
#include <iostream>
#include <llvm/IR/Function.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <string>
#include <vector>

namespace kaleidoscope {

using namespace llvm;

// clase base para todo nodo de expresión
class ExprAST {
public:
  ExprAST(kaleidoscope::Codegen &codegen);
  virtual ~ExprAST() = default;
  // para tener en cuenta más adelante, esta clase debería tener un campo
  // de "tipo", para los tipos de datos según nodo...
  virtual Value *codegen() = 0;
  // Value de LLVM es la clase que se usa para representar todo el tema del
  // Static Single Assignment (SSA) -> más info ->
  // https://en.wikipedia.org/wiki/Static_single-assignment_form

  // el mismo tuto recomienda no agregar esos métodos virtuales, sino usar algún
  // patrón como "Visitor Pattern" como alternativa.
protected:
  kaleidoscope::Codegen &cn;
};

// nodos para numerales, p.e. "1.0"
class NumberExprAST : public ExprAST {
  double val; // valor numérico

public:
  NumberExprAST(double val, kaleidoscope::Codegen &codegen);
  Value *codegen() override;
};

// nodos para referencias a varibales, p.e. "foo"
class VariableExprAST : public ExprAST {
  std::string name;

public:
  VariableExprAST(const std::string &name, kaleidoscope::Codegen &codegen);
  Value *codegen() override;
};

// nodos para operadores binarios
class BinaryExprAST : public ExprAST {
  char op;
  std::unique_ptr<ExprAST> LHS, RHS;

public:
  BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS,
                std::unique_ptr<ExprAST> RHS, kaleidoscope::Codegen &codegen);
  Value *codegen() override;
};

// nodo para llamados de funciones
class CallExprAST : public ExprAST {
  std::string callee; // no permite que se pasen números tal cuál como parámetro
  std::vector<std::unique_ptr<ExprAST>> args;

public:
  CallExprAST(const std::string &callee,
              std::vector<std::unique_ptr<ExprAST>> args,
              kaleidoscope::Codegen &codegen);
  Value *codegen() override;
};

// nodo para prototipo (de una función) -> se dice, nombre y argumentos
class PrototypeAST {
  std::string name;
  std::vector<std::string> args;
  kaleidoscope::Codegen &cn;

public:
  PrototypeAST(const std::string &name, std::vector<std::string> args,
               kaleidoscope::Codegen &codegen);
  std::string getName();
  Function *codegen();
};

// nodo para definición de función
class FunctionAST {
  std::unique_ptr<PrototypeAST> proto;
  std::unique_ptr<ExprAST> body;
  kaleidoscope::Codegen &cn;

public:
  FunctionAST(std::unique_ptr<PrototypeAST> proto,
              std::unique_ptr<ExprAST> body, kaleidoscope::Codegen &codegen);
  Function *codegen();
};

} // namespace kaleidoscope
