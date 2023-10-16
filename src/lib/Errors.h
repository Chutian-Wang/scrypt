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
 * Children of this custom Error class should be thrown
 * wherever exceptions occur. Do not throw a std::exception!
*/
class ScryptError: public std::exception{
public:
    Token tok;
    const char* msg;
    ScryptError() {};
    virtual ~ScryptError() {};
    virtual const char* what() const noexcept = 0;
};

class SyntaxError: public ScryptError {
public:
    SyntaxError(const Token& tok);
    virtual ~SyntaxError();
    virtual const char* what() const noexcept;
};

class UnexpTokError: public ScryptError {
public:
    UnexpTokError(const Token& tok);
    virtual ~UnexpTokError();
    virtual const char* what() const noexcept;
};

class DivByZero: public ScryptError {
public:
    DivByZero();
    virtual ~DivByZero() {};
    virtual const char* what() const noexcept;
};

#endif