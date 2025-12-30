#include "ast.hpp"
#include <vector>


class ConstFold {
public:
  ConstFold();
  ConstFold(std::vector<Stmt> &&s);

  void const_fold();
  Expr const_fold_expr(Expr e);
private:
  std::vector<Stmt> stmts;
};
