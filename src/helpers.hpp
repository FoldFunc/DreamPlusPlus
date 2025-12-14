#pragma once

#include <string>
#include "lexer.hpp"
[[noreturn]] void case_error(const std::string &msg);
std::string get_file(const std::string &filename);
void read_file(const std::string &file_contents);
void read_tokens(const std::vector<Token> &tokens);
