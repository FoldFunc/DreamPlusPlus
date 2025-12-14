#pragma once

#include "lexer.hpp"
#include <variant>
#include <vector>
#include <memory>

struct Scope;
struct Return;
struct IntLit;

using Stmt = std::variant<
  std::unique_ptr<Scope>,
  std::unique_ptr<Return>
>;

using Expr = std::variant<IntLit>;

struct IntLit {
  int value;
};

struct Scope {
  std::vector<Stmt> body;
};

struct Return {
  Expr value;
};

class Ast {
public:
  Ast();
  Ast(const std::vector<Token> &tokens_vec);
  std::vector<Stmt> parse();
private:
  std::vector<Token> tokens;
};

