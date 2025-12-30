#include "ast.hpp"
#include "../helpers/helpers.hpp"
#include "../lexer/lexer.hpp"
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
template <typename T>
bool Ast::consume() {
  if (i >= static_cast<int>(tokens.size())) {
    case_error("Unexpected end of input");
  }
  const Token &token = tokens[i];
  if (std::holds_alternative<T>(token)) {
    i++;
    return true;
  }
  case_error("Expected: " + token_type_name<T>() + ", got: " + token_to_string(token));
  return false;
}
Expr Ast::parse_expr() {
  Expr left = parse_primary();

  while (i < tokens.size()) {
    Token current_token = tokens[i];
    if (std::holds_alternative<Plus>(current_token)) {
      consume<Plus>();
      Expr right = parse_primary();

      auto binop = BinOp();
      binop.op = BinOpKind::Add;
      binop.left = std::make_unique<Expr>(std::move(left));
      binop.right = std::make_unique<Expr>(std::move(right));

      left = Expr{ std::make_unique<BinOp>(std::move(binop))};
    }else if (std::holds_alternative<Minus>(current_token)) {
      consume<Minus>();
      Expr right = parse_primary();

      auto binop = BinOp();
      binop.op = BinOpKind::Sub;
      binop.left = std::make_unique<Expr>(std::move(left));
      binop.right = std::make_unique<Expr>(std::move(right));

      left = Expr{ std::make_unique<BinOp>(std::move(binop))};
    } else if (std::holds_alternative<Mul>(current_token)) {
      consume<Mul>();
      Expr right = parse_primary();

      auto binop = BinOp();
      binop.op = BinOpKind::Mul;
      binop.left = std::make_unique<Expr>(std::move(left));
      binop.right = std::make_unique<Expr>(std::move(right));

      left = Expr{ std::make_unique<BinOp>(std::move(binop))};

    } else if (std::holds_alternative<Div>(current_token)) {
      consume<Div>();
      Expr right = parse_primary();

      auto binop = BinOp();
      binop.op = BinOpKind::Div;
      binop.left = std::make_unique<Expr>(std::move(left));
      binop.right = std::make_unique<Expr>(std::move(right));

      left = Expr{ std::make_unique<BinOp>(std::move(binop))};
    } else {
      break;
   }
  }
  return left;
}
Expr Ast::parse_primary() {
  Token current_token = tokens[i];
  if (auto *num = std::get_if<Number>(&current_token)) {
    auto lit = IntLit();
    lit.value = num->value;
    consume<Number>();
    return Expr{ lit };
  } else if (auto *variable = std::get_if<Identifier>(&current_token)) {
    auto var = VarMut();
    var.name = variable->name;
    consume<Identifier>();
    return Expr{ var };
  } else if (auto *function = std::get_if<FunctionCall>(&current_token)) {
    auto func = FuncCall();
    func.name = function->function_name;
    consume<Identifier>();
    return Expr{ func };
  }
  case_error("Expected expression");
}
Stmt Ast::parse_return(bool is_main) {
  if (is_main) {
    auto return_val = std::make_unique<RetMain>();
    auto current_token = tokens[i];
    auto val = parse_expr();
    return_val->value = std::move(val);
    consume<SColon>();
    Stmt stmt = std::move(return_val);
    return stmt;
  } else {
    auto return_val = std::make_unique<Ret>();
    auto current_token = tokens[i];
    auto val = parse_expr();
    return_val->value = std::move(val);
    consume<SColon>();
    Stmt stmt = std::move(return_val);
    return stmt;
  }
}
Stmt Ast::parse_define() {
  auto define_val = std::make_unique<Def>();
  auto current_token = tokens[i];
  if (std::holds_alternative<Identifier>(current_token)) {
    if (auto *kw = std::get_if<Identifier>(&current_token)) {
      std::string name = kw->name;
      define_val->name = name;
      consume<Identifier>();
    }
  } else {
    case_error("Invalid in define.");
  }
  consume<Eq>();
  auto expr = parse_expr();
  define_val->value = std::move(expr);
  Stmt stmt = std::move(define_val);
  consume<SColon>();
  return stmt;

}
Stmt Ast::parse_scope(bool is_main) {
  std::vector<Stmt> scope;
  auto current_token = tokens[i];
  while (!std::holds_alternative<RBracket>(current_token)) {
    current_token = tokens[i];
    if (std::holds_alternative<Keyword>(current_token)) {
      if (auto *kw = std::get_if<Keyword>(&current_token)) {
        Keywords keyword = kw->keyword;
        if (keyword == Keywords::Function) {
          consume<Keyword>();
          Stmt function = parse_function();
          scope.push_back(std::move(function));
        } else if (keyword == Define) {
          consume<Keyword>();
          Stmt define = parse_define();
          scope.push_back(std::move(define));
        } else if (keyword == Return) {
          consume<Keyword>();
          Stmt return_val = parse_return(is_main);
          scope.push_back(std::move(return_val));
        }
      }
    }
    else if (std::holds_alternative<RBracket>(current_token)) {
      break;
    }
    else if (std::holds_alternative<LBracket>(current_token)){
      consume<LBracket>();
      Stmt inner = parse_scope(is_main);
      scope.push_back(std::move(inner));
    }
  }
  consume<RBracket>();
  return Stmt{
    std::make_unique<Scope>(Scope{ std::move(scope) })
  };
}
Stmt Ast::parse_function() {
  auto function = std::make_unique<Func>();
  auto name_parse = tokens[i];
  if (std::holds_alternative<FunctionCall>(name_parse)) {
    if (auto *kw = std::get_if<FunctionCall>(&name_parse)) {
      std::string name = kw->function_name;
      function->name = name;
    } else {
      case_error("Invalid after a function declaretion.");
    }
  } else {
    case_error("Invalid after a function declaration...");
  }
  consume<FunctionCall>();
  consume<LBracket>();
  Stmt body;
  if (function->name == "main") {
    body = parse_scope(true);
  } else {
    body = parse_scope(false);
  }
  function->body = std::move(body);
  Stmt stmt = std::move(function);
  return stmt;
}
std::vector<Stmt> Ast::parse() {
  std::vector<Stmt> result;
  while (i < tokens.size()) {
    auto current_token = tokens[i]; 
    if (std::holds_alternative<Keyword>(current_token)) {
      if (auto *kw = std::get_if<Keyword>(&current_token)) {
        if (kw->keyword == Keywords::Function) {
          consume<Keyword>();
          result.push_back(parse_function());
        } else if (kw->keyword == Keywords::Define) {
          consume<Keyword>();
          result.push_back(parse_define());
        } else if (kw->keyword == Keywords::Return) {
          consume<Keyword>();
          result.push_back(parse_return(false));
        } else {
          case_error("Not supported for now");
        }
      }
    } else {
      case_error("Invalid in parse");
    }
  }
  return result;
}
