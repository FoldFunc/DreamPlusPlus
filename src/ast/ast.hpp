#pragma once

#include "../lexer/lexer.hpp"
#include <variant>
#include <vector>
#include <memory>

struct Scope;
struct Ret;
struct RetMain;
struct IntLit;
struct VarMut;
struct Func;
struct Def;
struct FuncCall;
struct BinOp;
using Stmt = std::variant<
  std::unique_ptr<Def>,
  std::unique_ptr<Func>,
  std::unique_ptr<Scope>,
  std::unique_ptr<Ret>,
  std::unique_ptr<RetMain>
>;

using Expr = std::variant<IntLit, VarMut, FuncCall, std::unique_ptr<BinOp>>;

enum class BinOpKind {
  Add, 
  Sub,
  Mul,
  Div
};

struct BinOp {
  BinOpKind op;
  std::unique_ptr<Expr> left;
  std::unique_ptr<Expr> right;
};
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
struct FuncCall {
  std::string name;
};
struct Def {
  std::string name;
  Expr value;
};
struct Scope {
  std::vector<Stmt> body;
};
struct RetMain {
  Expr value;
};
struct Ret {
  Expr value;
};

class Ast {
public:
  Ast();
  Ast(const std::vector<Token> &tokens_vec);
  std::vector<Stmt> parse();
  Stmt parse_scope(bool is_main);
  Stmt parse_return(bool is_main);
  Stmt parse_define();
  Stmt parse_function();
  Expr parse_expr();
  Expr parse_primary();
  template <typename T>
  bool consume();
private:
  std::vector<Token> tokens;
  int i = 0;
  int scope_count = 0;
};

