#include "Errors.h"
#include "Token.h"
#include <iostream>
#include <sstream>

ScryptError::~ScryptError() {
    delete this->msg;
}

SyntaxError::SyntaxError(const Token& tok) {
    this->tok = tok;
    std::ostringstream oss;
    oss << "Syntax error on line " << tok.row << " column " <<
        tok.column << '.' << '\n';
    size_t strlen = oss.str().size() + 1;
    this->msg = new char[strlen];
    memcpy((void*)msg, oss.str().c_str(), strlen);
}

const char* SyntaxError::what() const noexcept {
    return this->msg;
}

UnexpTokError::UnexpTokError(const Token& tok) {
    this->tok = tok;
    std::ostringstream oss;
    oss << "Unexpected token at line " << tok.row << " column " <<
        tok.column << ": " << tok.text << '\n';
    size_t strlen = oss.str().size() + 1;
    this->msg = new char[strlen];
    memcpy((void*)msg, oss.str().c_str(), strlen);
}

const char* UnexpTokError::what() const noexcept {
    return this->msg;
}

DivByZero::DivByZero() {
    this->tok = tok;
    this->msg = nullptr;
}

const char* DivByZero::what() const noexcept {
    return "Runtime error: division by zero.\n";
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