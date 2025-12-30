#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "helpers/helpers.hpp"
#include "lexer/lexer.hpp"
#include "ast/ast.hpp"
#include "builder/builder.hpp"
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
  auto ast_tokens = ast.parse();
  read_ast_tokens(ast_tokens);

  Builder builder(std::move(ast_tokens));
  const std::vector<std::string> asm_lines = builder.build_asm();
  for (const std::string line : asm_lines)  {
    std::cout << line << "\n";
  }
  std::ofstream out_file("compiler_build/out.asm");
  if (!out_file) {
    case_error("Error creating a file");
  }
  for (const std::string line : asm_lines) {
    out_file << line << "\n";
  }
  out_file.flush();
  out_file.close();
  std::string asm_file = "compiler_build/out.asm";
  std::string obj_file = "compiler_build/out.o";
  std::string exe_file = "compiler_build/out";

  std::string assemble_cmd = "nasm -f elf64 " + asm_file + " -o " + obj_file;
  if (std::system(assemble_cmd.c_str()) != 0) {
    case_error("Assembling failed");
    return EXIT_FAILURE;
  }
  
  std::string link_cmd = "ld " + obj_file + " -o " + exe_file;
  if (std::system(link_cmd.c_str()) != 0) {
    case_error("Linking failed");
    return EXIT_FAILURE;
  }
  std::string chmod_cmd = "chmod +x " + exe_file;
  if (std::system(chmod_cmd.c_str()) != 0) {
    case_error("Chmoding failed");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

