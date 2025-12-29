#include "ast.hpp"
#include <utility>
#include <vector>
class Builder {
public:
  Builder();
  Builder(std::vector<Stmt> &&stmt_tokens);
  std::vector<std::string> build_asm();
  void build_function(Func f);
  void build_scope(const Stmt &s);
  std::string build_expr(Expr e);
  void push(const std::string &s);
private:
  std::vector<std::string> lines;
  std::vector<Stmt> s;
  int indent = 0;
  std::vector<std::pair<std::string, int>> virtual_stack;
  int stack_pointer = 0;
};
