#include "ast.hpp"
#include "builder.hpp"
#include <vector>
Builder::Builder()
  : s(), lines() {}
Builder::Builder(std::vector<Stmt> &&stmt_tokens)
  : s(std::move(stmt_tokens)), lines() {}
std::vector<std::string> Builder::build_asm() {
  return lines;
}
