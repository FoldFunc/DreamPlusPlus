#include "../ast/ast.hpp"
#include "../helpers/helpers.hpp"
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
      } else if constexpr (std::is_same_v<T, FuncCall>) {
        lines.push_back(std::format("{}call {}", std::string(indent, ' '), value.name));
        return_val = "rax";
      } else if constexpr (std::is_same_v<T, std::unique_ptr<BinOp>>) {
        std::string left_reg = build_expr(std::move(*value->left));
        lines.push_back(std::format("{}push {}", std::string(indent, ' '), left_reg));

        std::string right_reg= build_expr(std::move(*value->right));

        lines.push_back(std::format("{}pop rcx", std::string(indent, ' ')));
        lines.push_back(std::format("{}mov rax, rcx", std::string(indent, ' ')));

        switch (value->op) {
          case BinOpKind::Add:
            lines.push_back(std::format("{}add rax, {}", std::string(indent, ' '), right_reg));
            break;
          case BinOpKind::Sub:
            lines.push_back(std::format("{}sub rax, {}", std::string(indent, ' '), right_reg));
            break;
          case BinOpKind::Mul:
            if (right_reg != "rbx") {
              lines.push_back(std::format("{}mov rbx, {}", std::string(indent, ' '), right_reg));
              lines.push_back(std::format("{}imul rax, rbx", std::string(indent, ' ')));
            } else {
              lines.push_back(std::format("{}imul rax, {}", std::string(indent, ' '), right_reg));
            }
            break;
          case BinOpKind::Div:
            lines.push_back(std::format("{}xor rdx, rdx", std::string(indent, ' ')));
            if (right_reg != "rbx") {
              lines.push_back(std::format("{}mov rbx, {}", std::string(indent, ' '), right_reg));
              lines.push_back(std::format("{}idiv rbx", std::string(indent, ' ')));
            } else {
              lines.push_back(std::format("{}idiv {}", std::string(indent, ' '), right_reg));
            }
            break;
        }
        return_val = "rax";
      } 
      else {
        case_error("Invalid while converting expr to assembly.");
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
        Scope &new_value = *value;
        for (const auto &stmt : new_value.body) {
          this->process_stmt(stmt, stack_pos);
        }
      } else {
        case_error("Should be a scope...");
      }
  }, s);
  
  for (int i = 0; i < stack_pos; i++) {
    virtual_stack.pop_back();
    stack_pointer += 8;
  }
}

void Builder::process_stmt(const Stmt &stmt, int &stack_pos) {
  std::visit([this, &stack_pos, &stmt] (const auto &stmt_val) {
      using T2 = std::decay_t<decltype(stmt_val)>;
      if constexpr (std::is_same_v<T2, std::unique_ptr<Def>>) {
        auto &new_stmt_val = *stmt_val;
        std::string name = new_stmt_val.name; 
        Expr expr = std::move(new_stmt_val.value);
        std::string asm_expr = this->build_expr(std::move(expr));
        this->lines.push_back(std::format("{}mov rax, {}", std::string(this->indent, ' '), asm_expr));
        this->lines.push_back(std::format("{}mov [rbp-{}], rax", std::string(this->indent, ' '), this->stack_pointer*-1));
        stack_pos++;
        this->push(name);
      } else if constexpr (std::is_same_v<T2, std::unique_ptr<Scope>>) {
        this->build_scope(stmt);
      } else if constexpr (std::is_same_v<T2, std::unique_ptr<RetMain>>) {
        auto &new_stmt_val = *stmt_val;
        Expr expr = std::move(new_stmt_val.value); 
        std::string asm_expr = this->build_expr(std::move(expr));
        this->lines.push_back(std::format("{}mov rax, 60", std::string(this->indent, ' ')));
        this->lines.push_back(std::format("{}mov rdi, {}", std::string(this->indent, ' '), asm_expr));
        this->lines.push_back(std::format("{}syscall", std::string(this->indent, ' ')));
      } else if constexpr (std::is_same_v<T2, std::unique_ptr<Ret>>) {
        auto &new_stmt_val = *stmt_val;
        Expr expr = std::move(new_stmt_val.value);
        std::string asm_expr = this->build_expr(std::move(expr));
        this->lines.push_back(std::format("{}mov rax, {}", std::string(this->indent, ' '), asm_expr));
        this->lines.push_back(std::format("{}ret", std::string(this->indent, ' ')));
      } else {
        case_error("Functions inside functions are not supported");
      }
  }, stmt);
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
    lines.insert(lines.end()-1, std::format("{}leave", std::string(indent, ' ')));
  } else {
    indent = 2;
    lines.push_back(std::format("{}:", f.name));
    build_scope(std::move(f.body));
    lines.insert(lines.end()-1, std::format("{}leave", std::string(indent, ' ')));
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
