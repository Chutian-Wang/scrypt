/**
 * This header contains all of the possible errors in this
 * project. Please **DO NOT** inline any exit(), throw, or
 * equivalent error handling or use any error handling
 * function outside of the definitions in this header.
*/

#ifndef ERRORS_H
#define ERRORS_H

#include "Token.h"
#include <iostream>

// Use these macros as exit code.
#define SYNTAX_ERR  1
#define UNEXP_TOK   2
#define DIV_BY_ZERO 3

/**
 * This custom Error class should be thrown wherever
 * exceptions occur. Do not throw a std::exception!
*/
class ScryptError {
public:
    Token tok;
    ScryptError() {};
    virtual std::string what() const = 0;
};

class SyntaxError: public ScryptError {
public:
    SyntaxError(const Token& tok);
    virtual std::string what() const;
};

class UnexpTokError: public ScryptError {
public:
    UnexpTokError(const Token& tok);
    virtual std::string what() const;
};

class DivByZero: public ScryptError {
public:
    DivByZero(const Token& tok);
    virtual std::string what() const;
};


// Legacy depreciated...
void syntax_err(Token tok);
void unexp_tok_err(Token tok);
void exp_tok_err(Token tok);
void div_by_zero_err();

#endif