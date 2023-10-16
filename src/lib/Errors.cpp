#include "Errors.h"
#include "Token.h"
#include <iostream>
#include <sstream>

SyntaxError::SyntaxError(const Token& tok) {
    this->tok = tok;
}

std::string SyntaxError::what() const {
    std::ostringstream oss;
    oss << "Syntax error on line " << tok.row << " column " <<
        tok.column << '.' << '\n';
    return oss.str();
}

UnexpTokError::UnexpTokError(const Token& tok) {
    this->tok = tok;
}

std::string UnexpTokError::what() const {
    std::ostringstream oss;
    oss << "Unexpected token at line " << tok.row << " column " <<
        tok.column << ": " << tok.text << '\n';
    return oss.str();
}

DivByZero::DivByZero(const Token& tok) {
    this->tok = tok;
}

std::string DivByZero::what() const {
    return std::string("Runtime error: division by zero.\n");
}

void syntax_err(Token tok) {
    std::cout <<
        "Syntax error on line " <<
        std::to_string(tok.row) <<
        " column " <<
        std::to_string(tok.column) <<
        '.' << std::endl;
    exit(1);
}

void unexp_tok_err(Token tok) {
    std::cout <<
        "Unexpected token at line " <<
        std::to_string(tok.row) <<
        " column " <<
        std::to_string(tok.column) <<
        ": " << tok.text << std::endl;
    exit(2);
}

void exp_tok_err(Token tok) {
    std::cout <<
        "Expecting token at line " <<
        std::to_string(tok.row) <<
        " column " <<
        std::to_string(tok.column) <<
        ": " << tok.text << std::endl;
    exit(2);
}

void div_by_zero_err() {
    std::cout << "Runtime error: division by zero." << std::endl;
    exit(3);
}