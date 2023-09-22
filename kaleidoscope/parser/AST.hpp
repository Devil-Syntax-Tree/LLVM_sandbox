#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// clase base para todo nodo de expresión
class ExprAST {
public:
  virtual ~ExprAST() = default;
  // para tener en cuenta más adelante, esta clase debería tener un campo
  // de "tipo", para los tipos de datos según nodo...
};

// nodos para numerales, p.e. "1.0"
class NumberExprAST : public ExprAST {
  double val; // valor numérico

public:
  NumberExprAST(double val);
};

// nodos para referencias a varibales, p.e. "foo"
class VariableExprAST : public ExprAST {
  std::string name;

public:
  VariableExprAST(const std::string &name);
};

// nodos para operadores binarios
class BinaryExprAST : public ExprAST {
  char op;
  std::unique_ptr<ExprAST> LHS, RHS;

public:
  BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS,
                std::unique_ptr<ExprAST> RHS);
};

// nodo para llamados de funciones
class CallExprAST : public ExprAST {
  std::string callee; // no permite que se pasen números tal cuál como parámetro
  std::vector<std::unique_ptr<ExprAST>> args;

public:
  CallExprAST(const std::string &callee,
              std::vector<std::unique_ptr<ExprAST>> args);
};

// nodo para prototipo (de una función) -> se dice, nombre y argumentos
class PrototypeAST {
  std::string name;
  std::vector<std::string> args;

public:
  PrototypeAST(const std::string &name, std::vector<std::string> args);
};

// nodo para definición de función
class FunctionAST {
  std::unique_ptr<PrototypeAST> proto;
  std::unique_ptr<ExprAST> body;

public:
  FunctionAST(std::unique_ptr<PrototypeAST> proto,
              std::unique_ptr<ExprAST> body);
};
