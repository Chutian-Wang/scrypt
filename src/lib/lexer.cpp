#include "token.h"
#include "lexer.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
using std::istringstream;

Lexer::Lexer(std::string &input): text(input){
    currRow = 1;
    currCol = 0;
}

void Lexer::addToken(TokenType type, const std::string &token){
    tokens.push_back(Token(type, token, currRow, currCol));
}

void Lexer::tokenize(std::string &input){
    std::string num;

    for (auto &token : input){
        currCol ++;

        if (std::isspace(token)){}
        else if (token == '('){
            addToken(TokenType::Lparen, "(");
        }
        else if (token == ')'){
            addToken(TokenType::Rparen, ")");
        }
        else if (token == '+'){
            addToken(TokenType::Operator, "+");
        }
        else if (token == '-'){
            addToken(TokenType::Operator, "-");
        }
        else if (token == '*'){
            addToken(TokenType::Operator, "*");
        }
        else if (token == '/'){
            addToken(TokenType::Operator, "/");
        }
        else if (std::isdigit(token) || ((token == '.') && (std::isdigit(input[currCol])))){
            num += token;
        }
        else if (std::isdigit(token) || ((token == '.') && (!std::isdigit(input[currCol])))){
            num += token;
            addToken(TokenType::Number, num);
        }
        else if (token == '\n'){
            currRow ++;
        }
        else{
            std::string str;
            str += token;
            throw Token(TokenType::Error, str, currRow, currCol);
        }
    }
    addToken(TokenType::END, "END");
}

// void Lexer::printTokens(){
    
// }

Token::Token(TokenType type, const std::string &text, int row, int column): chr(text){
    this->type   = type;
    this->row    = row;
    this->column = column;
}