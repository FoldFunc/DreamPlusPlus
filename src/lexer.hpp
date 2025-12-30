#pragma once
#include <string>
#include <variant>
#include <vector>
enum Keywords {
  Function,
  Define,
  Return, // Need to add it in .cpp
};
struct FunctionCall {
  std::string function_name;
};
struct Identifier {
  std::string name;
};
struct Number {
  int value;
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
      FunctionCall,
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
