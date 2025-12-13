#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "helpers.hpp"
[[noreturn]] void case_error(const std::string &msg) {
  std::cerr << "Error: " << msg << "\n";
  throw std::runtime_error(msg);
}

std::vector<std::vector<std::string>> get_file(const std::string &filename) {
  std::vector<std::vector<std::string>> return_vector;
  std::ifstream file(filename);
  if (!file) {
    case_error("Invalid file name: " + filename);
  }

  std::string line;
  while (std::getline(file, line)) {
    std::vector<std::string> line_contents;
    std::istringstream iss(line);
    std::string word;
    while (iss >> word) {
      line_contents.push_back(word);
    }
    return_vector.push_back(line_contents);
  }

  return return_vector;
}

void read_file(const std::vector<std::vector<std::string>> &file_contents) {
  for (const auto &line : file_contents) {
    for (const auto &word : line) {
      std::cout << word << "\n";
    }
  }
}
