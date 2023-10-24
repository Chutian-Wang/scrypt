/**
 * This header contains all of the possible errors in this
 * project. Please **DO NOT** inline any exit(), throw, or
 * equivalent error handling or use any error handling
 * function outside of the definitions in this header.
 */

#ifndef ERRORS_H
#define ERRORS_H

#include <iostream>

#include "Token.h"

// Use these macros as exit code.
#define SYNTAX_ERR 1
#define UNEXP_TOK 2
#define SCRYPT_RUNTIME 3
#define DIV_BY_ZERO SCRYPT_RUNTIME
#define UNKNOWN_IDENT SCRYPT_RUNTIME

/**
 * Children of this custom Error class should be thrown
 * wherever exceptions occur. Do not throw a std::exception!
 */
class ScryptError : public std::exception {
 public:
  Token tok;
  const char *msg;
  int exit_code;

  ScryptError(){};
  virtual ~ScryptError(){};
  virtual const char *what() const noexcept = 0;
  static int handle(std::ostream &cout, const ScryptError &err);
};

class ScryptRuntimeError : public ScryptError {
 public:
  ScryptRuntimeError(){};
  virtual ~ScryptRuntimeError(){};
  virtual const char *what() const noexcept = 0;
};

class SyntaxError : public ScryptError {
 public:
  SyntaxError(const Token &tok);
  virtual ~SyntaxError();
  virtual const char *what() const noexcept;
};

class UnexpTokError : public ScryptError {
 public:
  UnexpTokError(const Token &tok);
  virtual ~UnexpTokError();
  virtual const char *what() const noexcept;
};

class DivByZero : public ScryptRuntimeError {
 public:
  DivByZero();
  // what() is static, no need to write destructor
  virtual ~DivByZero(){};
  virtual const char *what() const noexcept;
};

// Shared TODO!
class UnknownIdent : public ScryptRuntimeError {
 public:
  UnknownIdent(const Token &tok);
  virtual ~UnknownIdent();
  virtual const char *what() const noexcept;
};

#endif