#pragma once

#include <string>
#include <vector>

[[noreturn]] void case_error(const std::string &msg);
std::vector<std::vector<std::string>> get_file(const std::string &filename);
void read_file(const std::vector<std::vector<std::string>> &file_contents);
