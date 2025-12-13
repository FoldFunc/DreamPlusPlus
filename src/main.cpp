#include <string>
#include "helpers.hpp"
int main(int argc, char *argv[]) {
  if (argc != 2) {
    case_error("Incorrect usage.\nCorrect usage: ./compiler <filename_to_compile>");
  }

  std::string filename = argv[1];
  auto file_contents = get_file(filename);
  read_file(file_contents);
  return EXIT_SUCCESS;
}

