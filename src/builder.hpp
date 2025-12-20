#include "ast.hpp"
#include <vector>
class Builder {
public:
  Builder();
  Builder(std::vector<Stmt> &&stmt_tokens);
  std::vector<std::string> build_asm();
private:
  std::vector<std::string> lines;
  std::vector<Stmt> s;
};
