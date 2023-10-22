#include "Errors.h"

#include <cstring>
#include <iostream>
#include <sstream>

#include "Token.h"

int ScryptError::handle(std::ostream &cout, const ScryptError &err) {
  cout << err.what() << std::endl;
  return err.exit_code;
}

SyntaxError::SyntaxError(const Token &tok) {
  this->exit_code = SYNTAX_ERR;
  this->tok = tok;
  std::ostringstream oss;
  oss << "Syntax error on line " << tok.row << " column " << tok.column << '.';
  size_t strlen = oss.str().size() + 1;
  this->msg = new char[strlen];
  memcpy((void *)msg, oss.str().c_str(), strlen);
}

SyntaxError::~SyntaxError() { delete[] this->msg; }

const char *SyntaxError::what() const noexcept { return this->msg; }

UnexpTokError::UnexpTokError(const Token &tok) {
  this->exit_code = UNEXP_TOK;
  this->tok = tok;
  std::ostringstream oss;
  oss << "Unexpected token at line " << tok.row << " column " << tok.column
      << ": " << tok.text;
  size_t strlen = oss.str().size() + 1;
  this->msg = new char[strlen];
  memcpy((void *)msg, oss.str().c_str(), strlen);
}

UnexpTokError::~UnexpTokError() { delete[] this->msg; }

const char *UnexpTokError::what() const noexcept { return this->msg; }

DivByZero::DivByZero() {
  this->exit_code = DIV_BY_ZERO;
  this->tok = tok;
  this->msg = nullptr;
}

const char *DivByZero::what() const noexcept {
  return "Runtime error: division by zero.";
}