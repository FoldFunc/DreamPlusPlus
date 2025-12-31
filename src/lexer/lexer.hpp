#pragma once
#include <string>
#include <variant>
#include <vector>
enum Types {
  Int,
  Ch,
};
enum Keywords {
  Function,
  Define,
  Return,
  As,
};
struct FunctionCall {
  std::string function_name;
};
struct Identifier {
  std::string name;
};
struct CharLexer {
  char value;
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
struct Plus{};
struct Minus{};
struct Mul{};
struct Div{};
struct Sq{};
struct Dq{};
using Token = std::variant<
      FunctionCall,
      Keyword,
      Types,
      Identifier, 
      Number,
      CharLexer,
      LBracket, 
      RBracket,
      LParent,
      RParent,
      Eq,
      Plus,
      Minus,
      Mul,
      Div,
      SColon,
      Sq,
      Dq
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
