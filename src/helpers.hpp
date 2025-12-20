#pragma once

#include <string>
#include <vector>
#include "ast.hpp"
#include "lexer.hpp"
[[noreturn]] void case_error(const std::string &msg);
std::string get_file(const std::string &filename);
void read_file(const std::string &file_contents);
void read_tokens(const std::vector<Token> &tokens);
void print_stmt(const Stmt &stmt, int ident);
void print_stmt(const Expr &expr, int ident);
static void ident(int n);
void read_ast_tokens(const std::vector<Stmt> &ast_tokens);
std::string token_to_string(const Token &tok);
std::string keyword_to_string(Keywords k);
template <typename T>
std::string token_type_name();
