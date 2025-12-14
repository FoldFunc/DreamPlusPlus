#include <string>
#include "helpers.hpp"
#include "lexer.hpp"
int main(int argc, char *argv[]) {
  if (argc != 2) {
    case_error("Incorrect usage.\nCorrect usage: ./compiler <filename_to_compile>");
  }
  std::string filename = argv[1];
  std::string const file_contents = get_file(filename);
  Lexer lexer(file_contents);
  lexer.print();
  const auto tokens = lexer.tokenize();
  read_tokens(tokens);
  return EXIT_SUCCESS;
}

