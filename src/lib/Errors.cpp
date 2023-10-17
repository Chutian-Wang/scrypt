#include "Errors.h"
#include "Token.h"
#include <iostream>
#include <sstream>
#include <cstring>

SyntaxError::SyntaxError(const Token& tok) {
    this->tok = tok;
    std::ostringstream oss;
    oss << "Syntax error on line " << tok.row << " column " <<
        tok.column << '.' << '\n';
    size_t strlen = oss.str().size() + 1;
    this->msg = new char[strlen];
    memcpy((void*)msg, oss.str().c_str(), strlen);
}

SyntaxError::~SyntaxError() {
    delete[] this->msg;
}

const char* SyntaxError::what() const noexcept {
    return this->msg;
}

UnexpTokError::UnexpTokError(const Token& tok) {
    this->tok = tok;
    std::ostringstream oss;
    oss << "Unexpected token at line " << tok.row <<
        " column " << tok.column << ": " << tok.text << '\n';
    size_t strlen = oss.str().size() + 1;
    this->msg = new char[strlen];
    memcpy((void*)msg, oss.str().c_str(), strlen);
}

UnexpTokError::~UnexpTokError() {
    delete[] this->msg;
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