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
  this->msg = nullptr;
}

const char *DivByZero::what() const noexcept {
  return "Runtime error: division by zero.";
}

UnknownIdent::UnknownIdent(const Token &tok) {
  this->exit_code = UNKNOWN_IDENT;
  this->tok = tok;
  std::ostringstream oss;
  oss << "Runtime error: unknown identifier " << tok.text;
  size_t strlen = oss.str().size() + 1;
  this->msg = new char[strlen];
  memcpy((void *)msg, oss.str().c_str(), strlen);
}

UnknownIdent::~UnknownIdent() { delete[] this->msg; }

const char *UnknownIdent::what() const noexcept { return this->msg; }

InvalidCond::InvalidCond() {
  this->exit_code = INVALID_COND;
  this->msg = nullptr;
}

const char *InvalidCond::what() const noexcept {
  return "Runtime error: condition is not a bool.";
}

InvalidOperand::InvalidOperand() {
  this->exit_code = INVALID_OPRAND;
  this->msg = nullptr;
}

const char *InvalidOperand::what() const noexcept {
  return "Runtime error: invalid operand type.";
}

InvalidAssignee::InvalidAssignee() {
  this->exit_code = INVALID_OPRAND;
  this->msg = nullptr;
}

const char *InvalidAssignee::what() const noexcept {
  return "Runtime error: invalid assignee.";
}

NotAFunction::NotAFunction() {
  this->exit_code = NOT_FUNCTION;
  this->msg = nullptr;
}

const char *NotAFunction::what() const noexcept {
  return "Runtime error: not a function.";
}

IncorrentArg::IncorrentArg() {
  this->exit_code = INCORRECT_ARG;
  this->msg = nullptr;
}

const char *IncorrentArg::what() const noexcept {
  return "Runtime error: incorrect argument count.";
}

IndexNotNumber::IndexNotNumber() {
  this->exit_code = NOT_NUMBER;
  this->msg = nullptr;
}

const char *IndexNotNumber::what() const noexcept {
  return "Runtime error: index is not a number.";
}

IndexNotInt::IndexNotInt() {
  this->exit_code = NOT_INT;
  this->msg = nullptr;
}

const char *IndexNotInt::what() const noexcept {
  return "Runtime error: index is not an integer.";
}

NotArray::NotArray() {
  this->exit_code = NOT_ARRAY;
  this->msg = nullptr;
}

const char *NotArray::what() const noexcept {
  return "Runtime error: not an array.";
}

IndexOutOfBounds::IndexOutOfBounds() {
  this->exit_code = INDEX_OUT_OF_BOUNDS;
  this->msg = nullptr;
}

const char *IndexOutOfBounds::what() const noexcept {
  return "Runtime error: index out of bounds.";
}

Underflow::Underflow() {
  this->exit_code = ARR_UNDERFLOW;
  this->msg = nullptr;
}

const char *Underflow::what() const noexcept {
  return "Runtime error: underflow.";
}