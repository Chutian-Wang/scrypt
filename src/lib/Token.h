#ifndef TOKEN_H
#define TOKEN_H

#include <map>
#include <string>

enum struct TokenType {
  NUMBER,
  OPERATOR,    // +, -, *, /
  LPAREN,      // (
  RPAREN,      // )
  ASSIGN,      // =
  IDENTIFIER,  // Series of letters, numbers, and underscores. They must start
               // with letters or underscores.
  END,
  ERR  // Any other
};

class Token {
 public:
  static const std::map<char, int> Precedence;

  TokenType type;
  std::string text;
  int row;
  int column;

  Token();
  Token(TokenType type, const std::string &text, int row, int column);
};

#endif  // TOKEN_H