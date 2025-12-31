#include "ast.hpp"
#include <vector>


class ConstFold {
public:
  ConstFold();
  ConstFold(std::vector<Stmt> &&s);

  std::vector<Stmt> const_fold();
  Expr const_fold_expr(Expr e);
  Stmt const_fold_scope(Stmt scope);
private:
  std::vector<Stmt> stmts;
};
