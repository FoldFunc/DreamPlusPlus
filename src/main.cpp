#include <string>
#include "helpers.hpp"
#include "lexer.hpp"
#include "ast.hpp"
#include "builder.hpp"
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

  Ast ast(tokens);
  const auto ast_tokens = ast.parse();
  read_ast_tokens(ast_tokens);

  Builder builder(std::move(ast_tokens));
  const auto asm_lines = builder.build_asm();

  return EXIT_SUCCESS;
}

