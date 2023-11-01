#ifndef TOKEN_H
#define TOKEN_H

#include <map>
#include <string>

enum struct TokenType {
  NUMBER,
  OPERATOR,    // *, /, %, +, -, <, <=, >, >=, ==, !=, &, ^, |, =
  LPAREN,      // (
  RPAREN,      // )
  LCBRACE,     // {
  RCBRACE,     // }
  IDENTIFIER,  // Series of letters, numbers, and underscores. They must start
               // with letters or underscores.
  BOOL,
  WHILE,
  IF,
  PRINT,
  ELSE,
  END,
  ERR  // Any other
};

static const std::map<std::string, int> p_map{
    {"=", 1},  {"|", 2}, {"^", 3},  {"&", 4}, {"==", 5},
    {"!=", 5}, {"<", 6}, {"<=", 6}, {">", 6}, {">=", 6},
    {"+", 7},  {"-", 7}, {"*", 8},  {"/", 8}, {"%", 8}};

class Token {
 public:
  static const std::map<char, int> Precedence;

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