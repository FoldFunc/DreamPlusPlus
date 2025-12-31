#include "../helpers/helpers.hpp"
#include "const_fold.hpp"
#include "ast.hpp"
#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

ConstFold::ConstFold(std::vector<Stmt> &&s) :
  stmts(std::move(s)) {}

Expr ConstFold::const_fold_expr(Expr e) {
  return std::visit([&](auto &&value) -> Expr {
    using T = std::decay_t<decltype(value)>;
    if constexpr (std::is_same_v<T, IntLit>) {
      return Expr{IntLit{value}};
    } else if constexpr (std::is_same_v<T, VarMut>) {
      return Expr{VarMut{value}};
    } else if constexpr (std::is_same_v<T, FuncCall>) {
      return Expr{FuncCall{value}};
    } else if constexpr (std::is_same_v<T, std::unique_ptr<BinOp>>) {
      Expr left = const_fold_expr(std::move(*value->left));
      Expr right = const_fold_expr(std::move(*value->right));
      
      const IntLit *left_int = std::get_if<IntLit>(&left);
      const IntLit *right_int = std::get_if<IntLit>(&right);
      
      if (left_int && right_int) {
        int result;
        switch (value->op) {
        case BinOpKind::Add: 
          result = left_int->value + right_int->value; 
          break;
        case BinOpKind::Sub: 
          result = left_int->value - right_int->value; 
          break;
        case BinOpKind::Mul: 
          result = left_int->value * right_int->value; 
          break;
        case BinOpKind::Div:
          if (right_int->value == 0) {
            case_error("Division by zero detected");
          }
          result = static_cast<int>(left_int->value / right_int->value);
          break;
        default:
          return Expr{std::make_unique<BinOp>(BinOp{
            value->op,
            std::make_unique<Expr>(std::move(left)),
            std::make_unique<Expr>(std::move(right))
          })};
        }
        return Expr{IntLit{result}};
      }
      
      return Expr{std::make_unique<BinOp>(BinOp{
        value->op,
        std::make_unique<Expr>(std::move(left)),
        std::make_unique<Expr>(std::move(right))
      })};
    }
    std::unreachable();
  }, std::move(e));
}

Stmt ConstFold::const_fold_scope(Stmt scope) {
  auto new_scope = std::make_unique<Scope>();
  
  std::visit([this, &new_scope](auto &&value) {
    using T = std::decay_t<decltype(value)>;
    if constexpr (std::is_same_v<T, std::unique_ptr<Scope>>) {
      for (auto &stmt : value->body) {
        new_scope->body.push_back(
          std::visit([this](auto &&inner_value) -> Stmt {
            using InnerT = std::decay_t<decltype(inner_value)>;
            if constexpr (std::is_same_v<InnerT, std::unique_ptr<Def>>) {
              auto new_def = std::make_unique<Def>();
              new_def->name = inner_value->name;
              new_def->value = const_fold_expr(std::move(inner_value->value));
              return Stmt{std::move(new_def)};
            } else {
              return Stmt{std::forward<decltype(inner_value)>(inner_value)};
            }
          }, std::move(stmt))
        );
      }
    }
  }, std::move(scope));
  
  return Stmt{std::move(new_scope)};
}

std::vector<Stmt> ConstFold::const_fold() {
  for (auto &stmt : stmts) {
    stmt = std::visit([this](auto &&value) -> Stmt {
      using T = std::decay_t<decltype(value)>;
      if constexpr (std::is_same_v<T, std::unique_ptr<Func>>) {
        auto new_func = std::make_unique<Func>();
        new_func->name = value->name; 
        new_func->body = const_fold_scope(std::move(value->body));
        return Stmt{std::move(new_func)};
      } else {
        return Stmt{std::forward<decltype(value)>(value)};
      }
    }, std::move(stmt));
  }
  return std::move(stmts);
}
