#pragma once

#include "lexer.hpp"
#include <variant>
#include <vector>
#include <memory>

struct Scope;
struct Ret;
struct IntLit;
struct VarMut;
struct Func;
struct Def;
using Stmt = std::variant<
  std::unique_ptr<Def>,
  std::unique_ptr<Func>,
  std::unique_ptr<Scope>,
  std::unique_ptr<Ret>
>;

using Expr = std::variant<IntLit, VarMut>;
struct VarMut {
  std::string name;
};
struct IntLit {
  int value;
};
struct Func {
  std::string name;
  Stmt body;
};
struct Def {
  std::string name;
  Expr value;
};
struct Scope {
  std::vector<Stmt> body;
};

struct Ret {
  Expr value;
};

class Ast {
public:
  Ast();
  Ast(const std::vector<Token> &tokens_vec);
  std::vector<Stmt> parse();
  Stmt parse_scope();
  Stmt parse_return();
  Stmt parse_define();
  Stmt parse_function();
  Expr parse_expr();
  template <typename T>
  bool consume();
private:
  std::vector<Token> tokens;
  int i = 0;
  int scope_count = 0;
};

