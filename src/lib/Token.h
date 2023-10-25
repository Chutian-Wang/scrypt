#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum struct TokenType {
  NUMBER,
  OPERATOR,    // +, -, *, /, =
  LPAREN,      // (
  RPAREN,      // )
  IDENTIFIER,  // Series of letters, numbers, and underscores. They must start
               // with letters or underscores.
  END,
  ERR  // Any other
};

class Token {
 public:
  TokenType type;
  std::string text;
  int row;
  int column;

  Token();
  Token(TokenType type, const std::string &text, int row, int column);
};

#endif  // TOKEN_H