#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <unordered_map>
#include <vector>

#include "Token.h"

class Lexer {
 public:
  // Default constructor for empty instance creation
  Lexer();

  // tokenize() is used to tokenize input S expression
  // into tokens, which are then stored in vector tokens
  // These tokens must be stored in **prefix order**,
  // e.g. {(, +, (, *, 1, 2, ), 3, )} parsed from the S-
  // expression (+ (* 1 2) 3) which evaluates to the
  // mathematical expression 1 + 2 * 3
  void tokenize(std::istream &input);

  const std::vector<Token> &get_tokens() const;

 private:
  std::vector<Token> tokens;
  int currRow;
  int currCol;
  static const std::unordered_map<std::string, TokenType> keywords;

  // Helper function
  // Used in tokenize() to add a new Token into the vector tokens
  // each time a new token is reached
  void add_token(TokenType type, const std::string &token, int row, int col);
  bool is_alpha(char c);
  bool is_alpha_num(char c);
  void read_comparison(char token, std::istream &input);
  void read_num(char token, std::istream &input);
  void read_identifier(char token, std::istream &input);
  void validate_num(int pos, const std::string &number);
};

#endif  // LEXER_H
