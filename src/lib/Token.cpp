#include "Token.h"

#include <map>

Token::Token() {
  this->type = TokenType::ERR;
  this->text = std::string();
  this->row = -1;
  this->column = -1;
}

Token::Token(TokenType type, const std::string &text, int row, int column) {
  this->type = type;
  this->text = text;
  this->row = row;
  this->column = column;
}

static const std::map<char, int> Precedence = {
    {'=', 0}, {'+', 2}, {'-', 2}, {'*', 3}, {'/', 3}, {'(', 1}, {')', 1},
};