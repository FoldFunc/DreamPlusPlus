#include <iostream>
#include <string>
#include <vector>
#include "lexer.hpp"
#include "helpers.hpp"

Lexer::Lexer()
  : file_contents() {}
Lexer::Lexer(const std::string &file) 
  : file_contents(file) {}
void Lexer::print() {
    std::cout << file_contents << "\n";
}
std::vector<Token> Lexer::tokenize() {
    std::vector<Token> result;
    std::size_t i = 0;

    auto is_ident_char = [](unsigned char c) {
        return std::isalnum(c) || c == '_';
    };

    while (i < file_contents.size()) {
        unsigned char c = file_contents[i];
        if (std::isspace(c)) {
            ++i;
            continue;
        }
        if (is_ident_char(c)) {
            std::string identifier;
            while (i < file_contents.size() &&
                   is_ident_char(static_cast<unsigned char>(file_contents[i]))) {
                identifier.push_back(file_contents[i]);
                ++i;
            }
            result.push_back(Identifier{identifier});
            continue;
        }
        switch (c) {
            case '{':
                result.push_back(LBracket{});
                break;
            case '}':
                result.push_back(RBracket{});
                break;
            default:
                case_error("Invalid char");
                break;
        }
        ++i;
    }
    return result;
}

