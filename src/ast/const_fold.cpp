#include "const_fold.hpp"
#include "ast.hpp"
#include <memory>
#include <type_traits>
#include <variant>
#include <vector>
ConstFold::ConstFold(std::vector<Stmt> &&s) :
  stmts(std::move(s)) {}
Expr ConstFold::const_fold_expr(Expr e) {
  std::visit([&] (const auto &value) {
      using T = std::decay_t<decltype(value)>;
      if constexpr (std::is_same_v<T, IntLit>) {
        return Expr{ std::move(IntLit{value}) };
      }
  }, e);
}
void ConstFold::const_fold() {
  for (auto &stmt : stmts) {
    std::visit([this, &stmt] (const auto &value){
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, Def>) {
          auto new_definition = std::make_unique<Def>();
          new_definition->value = std::move(const_fold_expr(value->value));
          Stmt new_def = std::move(new_definition);
          stmt = std::move(new_def);
        } else {
          return;
        }
    }, stmt);
  }
}
