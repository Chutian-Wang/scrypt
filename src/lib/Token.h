#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <map>

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

static const std::map<char, int> p_map {
  {'=', 1},
  {'+', 2},
  {'-', 2},
  {'*', 3},
  {'/', 3}
};

class Token {
 public:
  TokenType type;
  std::string text;
  int row;
  int column;

  Token();
  Token(TokenType type, const std::string &text, int row, int column);

  bool is_binary() const;
  int get_p() const;
};

#endif  // TOKEN_H