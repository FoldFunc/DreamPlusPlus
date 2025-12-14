#include "ast.hpp"
#include <vector>
Ast::Ast() 
  : tokens() {}
Ast::Ast(const std::vector<Token> &token_vec)
  : tokens(token_vec) {}
std::vector<Stmt> Ast::parse() {
  std::vector<Stmt> result;
  return result;
}
