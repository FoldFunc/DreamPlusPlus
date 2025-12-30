#include "ast.hpp"
#include "helpers.hpp"
#include "builder.hpp"
#include <algorithm>
#include <format>
#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>
Builder::Builder()
  : s(), lines() {}
Builder::Builder(std::vector<Stmt> &&stmt_tokens)
  : s(std::move(stmt_tokens)), lines() {}
std::string Builder::build_expr(Expr e) {
  std::string return_val;
  std::visit([this, &return_val] (const auto &value) {
      using T = std::decay_t<decltype(value)>;
      if constexpr (std::is_same_v<T, IntLit>) {
        return_val = std::format("{}", value.value);
      } else if constexpr (std::is_same_v<T, VarMut>) {
        std::string name = value.name; 
        auto it = std::find_if(virtual_stack.begin(), virtual_stack.end(),
            [&name](const std::pair<std::string, int> &p) {
              return p.first == name;
          });
        if (it != virtual_stack.end()) {
          int value = it->second;
          lines.push_back(std::format("{}mov rbx, [rbp-{}]", std::string(indent, ' '), value*-1));
          return_val = "rbx";
        } else {
          std::string case_err_str = std::format("No such variable as: {}", name);
          case_error(case_err_str);
        }
      }
  }, e);
  return return_val;
}
void Builder::push(const std::string &s) {
  virtual_stack.emplace_back(s, stack_pointer);
  stack_pointer -= 8;
}
void Builder::build_scope(const Stmt &s) {
  int stack_pos = 0;
  std::visit([this, &stack_pos] (const auto &value) {
      using T = std::decay_t<decltype(value)>;
      if constexpr (std::is_same_v<T, std::unique_ptr<Scope>>) {
        Scope new_value = std::move(*value);
        for (const auto &stmt : new_value.body) {
          std::visit([this, &stmt, &stack_pos] (const auto &stmt_val) {
              using T2 = std::decay_t<decltype(stmt_val)>;
              if constexpr (std::is_same_v<T2, std::unique_ptr<Def>>) {
                auto new_stmt_val = std::move(*stmt_val);
                std::string name = new_stmt_val.name; 
                Expr expr = new_stmt_val.value;
                std::string asm_expr = build_expr(expr);
                lines.push_back(std::format("{}mov rax, {}", std::string(indent, ' '), asm_expr));
                lines.push_back(std::format("{}mov [rbp-{}], rax", std::string(indent, ' '), stack_pointer*-1));
                stack_pos++;
                push(name);
              } else if constexpr (std::is_same_v<T2, std::unique_ptr<Scope>>) {
                  build_scope(std::move(stmt)); // AHHHHHHHHHHHHHH
              } else if constexpr (std::is_same_v<T2, std::unique_ptr<Ret>>) {
                  auto new_stmt_val = std::move(*stmt_val);
                  Expr expr = new_stmt_val.value; 
                  std::string asm_expr = build_expr(expr);
                  lines.push_back(std::format("{}mov rax, 60", std::string(indent, ' ')));
                  lines.push_back(std::format("{}mov rdi, {}", std::string(indent, ' '), asm_expr));
                  lines.push_back(std::format("{}syscall", std::string(indent, ' ')));
              } else {
                  case_error("Functions inside functions are not supported");
              }
          }, stmt);
        }
      } else {
        case_error("Should be a scope...");
      }
  }, s);
  for (int i = 0;i<stack_pos;i++) {
    virtual_stack.pop_back();
    stack_pointer += 8;
  }
}
void Builder::build_function(Func f) {
  if (f.name == "main") {
    lines.push_back("global _start");
    lines.push_back("_start:");
    indent = 2;
    lines.push_back(std::format("{}push rbp", std::string(indent, ' ')));
    lines.push_back(std::format("{}mov rbp, rsp", std::string(indent, ' ')));
    lines.push_back(std::format("{}sub rsp, 16", std::string(indent, ' ')));
    build_scope(std::move(f.body));
  } else {
    lines.push_back(std::format("{}:", f.name));
    build_scope(std::move(f.body));
    indent = 2;
  }
}
std::vector<std::string> Builder::build_asm() {
  int i = 0;
  lines.push_back("section .text");
  for (const auto &stmt : s) {
    std::visit([this](const auto &value) {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, std::unique_ptr<Func>>) {
          std::cout << "Building a function\n";
          build_function(std::move(*value));
        } else {
          std::string case_err_str = std::format("Expected a function parsed for now: {}\nType of the Stmt: {}\n", lines, typeid(T).name());  
          case_error(case_err_str);
        }
    }, stmt);
  }
  return lines;
}
