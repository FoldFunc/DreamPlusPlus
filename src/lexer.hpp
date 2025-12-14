#pragma once
#include <string>
#include <variant>
#include <vector>
enum Keywords {
  Function,
  Define,
};
struct Identifier {
  std::string name;
};
struct Number {
  std::string value;
};
struct Keyword {
  Keywords keyword;
};
struct LBracket {};
struct RBracket {};
struct LParent {};
struct RParent {};
struct Eq {};
struct SColon{};
using Token = std::variant<
      Keyword,
      Identifier, 
      Number,
      LBracket, 
      RBracket,
      LParent,
      RParent,
      Eq,
      SColon
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
