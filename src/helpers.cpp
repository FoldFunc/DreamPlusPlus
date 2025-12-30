#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>
#include "helpers.hpp"
#include "ast.hpp"
#include "lexer.hpp"
[[noreturn]] void case_error(const std::string &msg) {
  std::cerr << "Error: " << msg << "\n";
  throw std::runtime_error(msg);
}

std::string get_file(const std::string &filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file) {
    case_error("Incorrect file name.");
  }
  return std::string(
      std::istreambuf_iterator<char>(file),
      std::istreambuf_iterator<char>()
      );
}

std::string keyword_to_string(Keywords k) {
  switch (k) {
    case Keywords::Function: return "function";
    case Keywords::Define:   return "define";
    case Keywords::Return:   return "return";
    default:                 return "<unknown keyword>";
  }
}
std::string token_to_string(const Token &tok) {
  return std::visit([](auto &&t) -> std::string {
      using T = std::decay_t<decltype(t)>;
      if constexpr (std::is_same_v<T, Identifier>) {
      return "Identifier(" + t.name + ")";
      }
      else if constexpr (std::is_same_v<T, Number>) {
      return "Number(" + std::to_string(t.value) + ")";
      }
      else if constexpr (std::is_same_v<T, Keyword>) {
      return "Keyword(" + keyword_to_string(t.keyword) + ")";
      }
      else if constexpr (std::is_same_v<T, SColon>) {
      return ";";
      }
      else if constexpr (std::is_same_v<T, LBracket>) {
      return "{";
      }
      else if constexpr (std::is_same_v<T, RBracket>) {
      return "}";
      }
      else if constexpr (std::is_same_v<T, LParent>) {
      return "("; 
      }
      else if constexpr (std::is_same_v<T, LParent>) {
      return ")"; 
      }
      else {
        return "<unknown token>"; 
      }
  }, tok);
}

template <typename T>
std::string token_type_name();

template <>
std::string token_type_name<FunctionCall>() { return "FunctionCall"; }

template <>
std::string token_type_name<Identifier>() { return "Identifier"; }

template <>
std::string token_type_name<Number>() { return "Number"; }

template <>
std::string token_type_name<Keyword>() { return "Keyword"; }

template <>
std::string token_type_name<SColon>() { return "';'"; }

template <>
std::string token_type_name<LBracket>() { return "'{'"; }

template <>
std::string token_type_name<RBracket>() { return "'}'"; }

template <>
std::string token_type_name<LParent>() { return "'('"; }

template <>
std::string token_type_name<RParent>() { return "')'"; }

template <>
std::string token_type_name<Eq>() { return "'='"; }

void read_file(const std::string &file_contents) {
  for (int i = 0;i<static_cast<int>(file_contents.length());i++) {
    std::cout << file_contents[i] << "\n";
  }
}
void read_ast_tokens(const std::vector<Stmt> &ast_tokens) {
  for (const auto &stmt : ast_tokens) {
    print_stmt(stmt, 0);
  }
}
static void ident(int n) {
  std::string spaces = "";
  for (int i = 0;i<n;i++) {
    spaces.push_back(' ');
  }
  std::cout << spaces;
}
void print_expr(const Expr &expr, int ident_level) {
  std::visit([](auto &&node) {
      using T = std::decay_t<decltype(node)>;
      if constexpr (std::is_same_v<T, IntLit>) {
        std::cout << node.value; 
      } else if constexpr (std::is_same_v<T, VarMut>) {
        std::cout << node.name;
      } else if constexpr (std::is_same_v<T, FuncCall>) {
        std::cout << node.name << "()";
      }
  }, expr);
}
void print_stmt(const Stmt &stmt, int ident_level) {
  std::visit([&]( auto &&node_ptr) {
      using T = std::decay_t<decltype(node_ptr)>;
      if constexpr (std::is_same_v<T, std::unique_ptr<Func>>) {
          ident(ident_level);
          std::cout << "Func: " << node_ptr->name << "\n";
          std::visit([&] (auto &&stmt_ptr) {
              using T2 = std::decay_t<decltype(stmt_ptr)>;
              if constexpr (std::is_same_v<T2, std::unique_ptr<Scope>>) {
                for (const auto &s : stmt_ptr->body) {
                  print_stmt(s, ident_level+1);
                }
              }
          }, node_ptr->body);
      }else if constexpr (std::is_same_v<T, std::unique_ptr<Def>>) {
        ident(ident_level);
        std::cout << "Def: " << node_ptr->name << " = ";
        print_expr(node_ptr->value, 0);
        std::cout << "\n";
      } else if constexpr (std::is_same_v<T, std::unique_ptr<Scope>>) {
        ident(ident_level);
        std::cout << "Scope" << "\n";
        for (const Stmt &s : node_ptr->body) {
        print_stmt(s, ident_level + 1);
      }
      } else if constexpr (std::is_same_v<T, std::unique_ptr<Ret>>) {
        ident(ident_level);
        std::cout << "Return: ";
        print_expr(node_ptr->value, 0);
        std::cout << "\n";
      } else if constexpr (std::is_same_v<T, std::unique_ptr<RetMain>>) {
        ident(ident_level);
        std::cout << "Return from main: ";
        print_expr(node_ptr->value, 0);
        std::cout << "\n";
      }
  }, stmt);
}
void read_tokens(const std::vector<Token> &tokens) {
  for (const auto &token : tokens) {
    std::visit([](auto &&t) {
        using T = std::decay_t<decltype(t)>;
        if constexpr (std::is_same_v<T, Identifier>) {
        std::cout << "<Identifier: " << t.name << ">\n";
        }
        else if constexpr (std::is_same_v<T, FunctionCall>) {
          std::cout << "<FunctionCall>\n";
        }
        else if constexpr (std::is_same_v<T, LBracket>) {
        std::cout << "<LeftBracket>\n";
        }
        else if constexpr (std::is_same_v<T, RBracket>) {
        std::cout << "<RightBracket>\n";
        }
        else if constexpr (std::is_same_v<T, RParent>) {
        std::cout << "<RightParentesis>\n";
        }
        else if constexpr (std::is_same_v<T, LParent>) {
        std::cout << "<LeftParentesis>\n";
        }
        else if constexpr (std::is_same_v<T, Eq>) {
        std::cout << "<Eaqule>\n";
        }
        else if constexpr (std::is_same_v<T, SColon>) {
          std::cout << "<SemiColon>\n";
        }
        else if constexpr (std::is_same_v<T, Number>) {
          std::cout << "<Number: " << t.value << ">" << "\n";
        }
        else if constexpr (std::is_same_v<T, Keyword>){
          if (t.keyword == 0) {
            std::cout << "<Keyword: Function>" << "\n";
          } else if (t.keyword == 1) {
            std::cout << "<Keyword: Define>" << "\n";
          } else if (t.keyword == 2) {
            std::cout << "<Keyword: Return>" << "\n";
          }
        }
    }, token);
  }
}

