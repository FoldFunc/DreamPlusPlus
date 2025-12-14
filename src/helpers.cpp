#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>
#include "helpers.hpp"
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

void read_file(const std::string &file_contents) {
  for (int i = 0;i<file_contents.length();i++) {
    std::cout << file_contents[i] << "\n";
  }
}
void read_tokens(const std::vector<Token> &tokens) {
  for (const auto &token : tokens) {
    std::visit([](auto &&t) {
      using T = std::decay_t<decltype(t)>;
      if constexpr (std::is_same_v<T, Identifier>) {
        std::cout << "<Identifier: " << t.name << ">\n";
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
      else if constexpr (std::is_same_v<T, Keyword>){
        if (t.keyword == 0) {
          std::cout << "<Keyword: Function>" << "\n";
        } else if (t.keyword == 1) {
          std::cout << "<Keyword: Define>" << "\n";
        }
      }
    }, token);
  }
}

