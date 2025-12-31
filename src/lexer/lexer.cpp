#include <cctype>
#include <charconv>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "lexer.hpp"
#include "../helpers/helpers.hpp"
Lexer::Lexer()
  : file_contents() {}
Lexer::Lexer(const std::string &file) 
  : file_contents(file) {}
  void Lexer::print() {
    std::cout << file_contents << "\n";
  }
std::vector<Token> Lexer::tokenize() {
  std::vector<Token> result;
  std::size_t i = 0;

  auto is_ident_char = [](unsigned char c) {
    return std::isalpha(c) || c == '_'; // For now variable names can't and won't be numbers
  };
  auto is_number_char = [](unsigned char c) {
    return std::isdigit(c);
  };
  while (i < file_contents.size()) {
    unsigned char c = file_contents[i];
    if (std::isspace(c)) {
      ++i;
      continue;
    }
    if (is_ident_char(c)) {
      std::string identifier;
      while (i < file_contents.size() &&
          is_ident_char(static_cast<unsigned char>(file_contents[i]))) {
        identifier.push_back(file_contents[i]);
        ++i;
      }

      std::unordered_map<std::string, Keywords> keyword_map = {
        {"function", Function},
        {"define", Define},
        {"return", Return},
        {"as", As},
      };

      auto it2 = keyword_map.find(identifier);
      if (it2 != keyword_map.end()) {
        result.push_back(Keyword(it2->second));
        continue;
      }

      if (file_contents[i] == '(' && file_contents[i+1] == ')') {
        result.push_back(FunctionCall{identifier});
        i+=2;
        continue;
      }

      std::unordered_map<std::string, Types> type_map = {
        {"int", Int},
      };

      auto it1 = type_map.find(identifier);
      if (it1 != type_map.end()) {
        result.push_back(Types(it1->second));
        continue;
      }

      result.push_back(Identifier{identifier});
      continue;
    }else if (is_number_char(c)) {
      std::string number;
      while (i <file_contents.size() &&
          is_number_char(static_cast<unsigned char>(file_contents[i]))) {
        number.push_back(file_contents[i]);
        ++i;
      }
      int val;
      auto [ptr, ec] = std::from_chars(number.data(), number.data() + number.size(), val);
      if (ec == std::errc{}) {
        result.push_back(Number{val});
        continue;
      } else {
        case_error("Invalid number");
      }
    }
    switch (c) {
      case '{':
        result.push_back(LBracket{});
        break;
      case '}':
        result.push_back(RBracket{});
        break;
      case '(':
        result.push_back(LParent{});
        break;
      case ')':
        result.push_back(RParent{});
        break;
      case '=':
        result.push_back(Eq{});
        break;
      case ';':
        result.push_back(SColon{});
        break;
      case '+':
        result.push_back(Plus{});
        break;
      case '-':
        result.push_back(Minus{});
        break;
      case '*':
        result.push_back(Mul{});
        break;
      case '/':
        result.push_back(Div{});
        break;
      default:
        case_error("Invalid char");
        break;
    }
    ++i;
  }
  return result;
}

