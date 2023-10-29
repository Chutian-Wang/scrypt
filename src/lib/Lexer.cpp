#include "Lexer.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Errors.h"
#include "Token.h"

Lexer::Lexer() {
  currRow = 1;
  currCol = 0;
}

void Lexer::add_token(TokenType type, const std::string &token, int row,
                      int col) {
  tokens.push_back(Token(type, token, row, col));
}

const std::vector<Token> &Lexer::get_tokens() const { return tokens; }

bool Lexer::is_alpha(char c) {
  // check if the token is a alphabet or _
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

bool Lexer::is_alpha_num(char c) {
  // check if the token is alphabet, _ or number
  return is_alpha(c) || std::isdigit(c);
}

const std::unordered_map<std::string, TokenType> Lexer::keywords = {
    {"false", TokenType::FALSE},
    {"true", TokenType::TRUE},
    {"if", TokenType::IF},
    {"while", TokenType::WHILE},
    {"print", TokenType::PRINT},
    {"else", TokenType::ELSE}
};

void Lexer::read_identifier(char token, std::istream &input) {
  // read and add identifier to tokens
  std::string id;
  char newToken;
  int pos = currCol;
  id += token;
  while (is_alpha_num(input.peek())) {
    input.get(newToken);
    id += newToken;
    currCol++;
  }
  if (keywords.find(id) == keywords.end()){
    add_token(TokenType::IDENTIFIER, id, currRow, pos);
  } else{
    add_token(keywords.at(id), id, currRow, pos);
  }
  
}

void Lexer::read_num(char token, std::istream &input) {
  // read number
  std::string num;
  int period2, temp(0);
  char newToken;

  num += token;
  while (std::isdigit(input.peek()) || (input.peek() == '.')) {
    input.get(newToken);
    num += newToken;
    currCol++;
    if (newToken == '.') {
      // if we encounter a period, we increment temp
      // and store the position of 2nd period (where
      // the error location should be) in period2
      if (temp == 0) {
        temp++;
      } else if (temp == 1) {
        period2 = currCol;
        temp++;
      }
    }
  }
  validate_num(period2, num);
}

void Lexer::validate_num(int pos, const std::string &number) {
  // if the number is completely read, we now check if
  // the number is valid by counting the number of period
  // inside the number
  int n = std::count(number.begin(), number.end(), '.');
  if (n == 0) {
    // if there is no period, just add the number
    add_token(TokenType::NUMBER, number, currRow,
              currCol - (number.length() - 1));
  } else if (n == 1) {
    // if there is one period, there are 2 instances when
    // the number would be invalid e.g.
    // 1) .9, error position = 1
    // 2) 9., error position = 3
    size_t idx = number.find('.');
    if (idx == 0) {
      throw SyntaxError(Token(TokenType::ERR, number, currRow,
                              currCol - (number.length() - 1)));
    } else if (idx == number.length() - 1) {
      throw SyntaxError(Token(TokenType::ERR, number, currRow, currCol + 1));
    } else {
      add_token(TokenType::NUMBER, number, currRow,
                currCol - (number.length() - 1));
    }
  } else {
    // if there are more than 1 periods, the number is definitely invalid
    // 1) ..9, error position = 1
    // 2) 9..9, error position = 3
    size_t idx = number.find('.');
    if (idx == 0) {
      throw SyntaxError(Token(TokenType::ERR, number, currRow,
                              currCol - (number.length() - 1)));
    } else {
      throw SyntaxError(Token(TokenType::ERR, number, currRow, pos));
    }
  }
}

bool match(char expected, char curr){
    if (expected != curr) return false;
    return true;
}

void Lexer::readComparison(char token, std::istream &input){
    std::string str;
    if (token == '>'){
      str = match('=', input.peek()) ? ">=" : "=";
    } else if (token == '<'){
      str = match('=', input.peek()) ? "<=" : "=";
    } else if (token == '='){
      str = match('=', input.peek()) ? "==" : "=";
    } 
    if (str.length() == 2){
      add_token(TokenType::OPERATOR, str, currRow, currCol);
      input.get();
      currCol ++;
    } else {
      add_token(TokenType::OPERATOR, str, currRow, currCol);
    }
}

void Lexer::tokenize(std::istream &input) {
  // num stores number, e.g. 8.8, 134.45
  // str is used mainly for throwing exception since Token()
  // only accepts string in the 2nd parameter
  // period2 keeps track of where the 2nd period is when there
  // is error such as 8..9
  // temp keeps track of how many periods we have encountered
  // token keeps track of the current character from input
  std::string str;
  char token;

  while (input.get(token)) {
    // For each character in the input, we check if it is one
    // of the operators we recognize or if it is a valid number
    // If it is not a valid number or not a operator that we
    // know of, we throw an error
    currCol++;
    if (token == '\n') {
      currRow++;
      currCol = 0;
    } else if (std::isspace(token)) {
    } else if (token == '(') {
      add_token(TokenType::LPAREN, "(", currRow, currCol);
    } else if (token == ')') {
      add_token(TokenType::RPAREN, ")", currRow, currCol);
    } else if (token == '}') {
      add_token(TokenType::RCBRACE, "}", currRow, currCol);
    } else if (token == '{') {
      add_token(TokenType::LCBRACE, "{", currRow, currCol);
    } else if (token == '+') {
      add_token(TokenType::OPERATOR, "+", currRow, currCol);
    } else if (token == '-') {
      add_token(TokenType::OPERATOR, "-", currRow, currCol);
    } else if (token == '*') {
      add_token(TokenType::OPERATOR, "*", currRow, currCol);
    } else if (token == '/') {
      add_token(TokenType::OPERATOR, "/", currRow, currCol);
    } else if (token == '%') {
      add_token(TokenType::OPERATOR, "%", currRow, currCol);
    } else if (token == '!') {
      input.get();
      add_token(TokenType::OPERATOR, "!=", currRow, currCol);
      currCol ++;
    } else if (token == '=') {
      readComparison(token, input);
    } else if (token == '<') {
      readComparison(token, input);
    } else if (token == '>') {
      readComparison(token, input);
    } else if (token == '&') {
      add_token(TokenType::OPERATOR, "&", currRow, currCol);
    } else if (token == '^') {
      add_token(TokenType::OPERATOR, "^", currRow, currCol);
    } else if (token == '|') {
      add_token(TokenType::OPERATOR, "|", currRow, currCol);
    } else if (std::isdigit(token)) {
      read_num(token, input);
    } else if (is_alpha(token)) {
      read_identifier(token, input);
    } else {
      str += token;
      throw SyntaxError(Token(TokenType::ERR, str, currRow, currCol));
      str = "";
    }
  }
  currCol++;
  add_token(TokenType::END, "END", currRow, currCol);
  return;
}