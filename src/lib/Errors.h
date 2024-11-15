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
#define INVALID_COND SCRYPT_RUNTIME
#define INVALID_OPRAND SCRYPT_RUNTIME
#define INVALID_ASSIGNEE SCRYPT_RUNTIME
#define NOT_FUNCTION SCRYPT_RUNTIME
#define INCORRECT_ARG SCRYPT_RUNTIME
#define NOT_NUMBER SCRYPT_RUNTIME
#define NOT_INT SCRYPT_RUNTIME
#define NOT_ARRAY SCRYPT_RUNTIME
#define INDEX_OUT_OF_BOUNDS SCRYPT_RUNTIME
#define ARR_UNDERFLOW SCRYPT_RUNTIME

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

class UnknownIdent : public ScryptRuntimeError {
 public:
  UnknownIdent(const Token &tok);
  virtual ~UnknownIdent();
  virtual const char *what() const noexcept;
};

class InvalidCond : public ScryptRuntimeError {
 public:
  InvalidCond();
  virtual ~InvalidCond(){};
  // what() is static, no need to write destructor
  virtual const char *what() const noexcept;
};

class InvalidOperand : public ScryptRuntimeError {
 public:
  InvalidOperand();
  virtual ~InvalidOperand(){};
  // what() is static, no need to write destructor
  virtual const char *what() const noexcept;
};

class InvalidAssignee : public ScryptRuntimeError {
 public:
  InvalidAssignee();
  virtual ~InvalidAssignee(){};
  // what() is static, no need to write destructor
  virtual const char *what() const noexcept;
};

class NotAFunction : public ScryptRuntimeError {
 public:
  NotAFunction();
  virtual ~NotAFunction(){};
  // what() is static, no need to write destructor
  virtual const char *what() const noexcept;
};

class IncorrentArg : public ScryptRuntimeError {
 public:
  IncorrentArg();
  virtual ~IncorrentArg(){};
  // what() is static, no need to write destructor
  virtual const char *what() const noexcept;
};

class IndexNotNumber : public ScryptRuntimeError {
 public:
  IndexNotNumber();
  virtual ~IndexNotNumber(){};
  // what() is static, no need to write destructor
  virtual const char *what() const noexcept;
};

class IndexNotInt : public ScryptRuntimeError {
 public:
  IndexNotInt();
  virtual ~IndexNotInt(){};
  // what() is static, no need to write destructor
  virtual const char *what() const noexcept;
};

class NotArray : public ScryptRuntimeError {
 public:
  NotArray();
  virtual ~NotArray(){};
  // what() is static, no need to write destructor
  virtual const char *what() const noexcept;
};

class IndexOutOfBounds : public ScryptRuntimeError {
 public:
  IndexOutOfBounds();
  virtual ~IndexOutOfBounds(){};
  // what() is static, no need to write destructor
  virtual const char *what() const noexcept;
};

class Underflow : public ScryptRuntimeError {
 public:
  Underflow();
  virtual ~Underflow(){};
  // what() is static, no need to write destructor
  virtual const char *what() const noexcept;
};

#endif