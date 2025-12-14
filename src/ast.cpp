/*
 * Note to self:
 * Create a consume function that will consume used tokens.
 */
#include "ast.hpp"
#include "helpers.hpp"
#include "lexer.hpp"
#include <charconv>
#include <memory>
#include <variant>
#include <vector>
int strint(std::string value) {
  int ret;
  auto [ptr, ec] = std::from_chars(value.data(), value.data() + value.size(), ret);
  if (ec == std::errc{}) {
    return ret;
  } else {
    case_error("Invalid in return expression.");
  }
}
Ast::Ast() 
  : tokens(), i(), scope_count() {}
Ast::Ast(const std::vector<Token> &token_vec)
  : tokens(token_vec), i(), scope_count() {}
Expr Ast::parse_expr() {
    Token current_token = tokens[i];

    if (auto* num = std::get_if<Number>(&current_token)) {
        auto lit = std::make_unique<IntLit>();
        lit->value = num->value;
        ++i;
        return Expr{ std::move(lit) };
    }

    case_error("Expected expression");
}
Stmt Ast::parse_return() {
  auto return_val = std::make_unique<Ret>();
  auto current_token = tokens[i];
  if (std::holds_alternative<Identifier>(current_token)) {
    if (auto *kw = std::get_if<Identifier>(&current_token)) {
      int value = strint(kw->name);
      return_val->value = value;
    }
  }
  Stmt stmt = std::move(return_val);
  i++;
  i++; // Consumes identifier and a semicolon
  return stmt;
}
Stmt Ast::parse_define() {
  auto define_val = std::make_unique<Def>();
  auto current_token = tokens[i];
  if (std::holds_alternative<Identifier>(current_token)) {
    if (auto *kw = std::get_if<Identifier>(&current_token)) {
      std::string name = kw->name;
      define_val->name = name;
      i++;
    }
  } else {
    case_error("Invalid in define.");
  }
  auto expr = parse_expr();
  define_val->value = expr;
  Stmt stmt = std::move(define_val);
  i++;
  i++;
  return stmt;

}
std::vector<Stmt> Ast::parse_scope() {
  scope_count++;
  std::vector<Stmt> scope;
  auto current_token = tokens[i];
  while (!std::holds_alternative<LBracket>(current_token) && scope_count == 0) {
    current_token = tokens[i];
    if (std::holds_alternative<Keyword>(current_token)) {
      if (auto *kw = std::get_if<Keyword>(&current_token)) {
        Keywords keyword = kw->keyword;
        if (keyword == Keywords::Function) {
          i++;
          Stmt function = parse_function();
          scope.push_back(function); // Basar
        } else if (keyword == Define) {
          i++;
          Stmt define = parse_define();
          scope.push_back(define);
        } else if (keyword == Return) {
          i++;
          Stmt return_val = parse_return();
          scope.push_back(return_val);
        }
      }
    }
    else {
      case_error("Not an someting idk.");
    }
  }
  scope_count--;
  return scope;
}
Stmt Ast::parse_function() {
  scope_count++;
  auto function = std::make_unique<Func>();
  auto name_parse = tokens[i];
  if (std::holds_alternative<Identifier>(name_parse)) {
    if (auto *kw = std::get_if<Identifier>(&name_parse)) {
      std::string name = kw->name; // Can and will blow you here
      function->name = name;
      i++;
    } else {
      case_error("Invalid after a function declaretion.");
    }
  } else {
    case_error("Invalid after a function declaration.");
  }
  i += 3; // Skip () to the first token of a scope for now;
  auto body = parse_scope();
  function->body = body;
  Stmt stmt = std::move(function);
  scope_count--;
  return stmt;
}
std::vector<Stmt> Ast::parse() {
  std::vector<Stmt> result;
  while (i < tokens.size()) {
    auto current_token = tokens[i]; 
    if (std::holds_alternative<Keyword>(current_token)) {
      if (auto *kw = std::get_if<Keyword>(&current_token)) {
        if (kw->keyword == Keywords::Function) {
          i++;
          result.push_back(parse_function());
        }
      }
    }
  }
  return result;
}
