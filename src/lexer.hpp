#pragma once
#include <string>
#include <variant>
#include <vector>
struct Identifier {
  std::string name;
};
struct LBracket {};
struct RBracket {};
using Token = std::variant<
      Identifier, 
      LBracket, 
      RBracket
>;
class Lexer {
public:
  Lexer();
  Lexer(const std::string &file);
  std::vector<Token> tokenize();
  void print();
private:
  std::string file_contents;
};
