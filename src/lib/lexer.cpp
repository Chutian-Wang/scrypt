#include "token.h"
#include "lexer.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
using std::istringstream;

Lexer::Lexer(std::string &input): text(input){
    // text = input;
    currRow = 1;
    currCol = 0;
}

void Lexer::addToken(TokenType type, const std::string &token){
    tokens.push_back(Token(type, token, currRow, currCol));
}

void Lexer::tokenize(std::string &input){
    std::string num;

    // try{
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
                addToken(Operator, "+");
            }
            else if (token == '-'){
                addToken(Operator, "-");
            }
            else if (token == '*'){
                addToken(Operator, "*");
            }
            else if (token == '/'){
                addToken(Operator, "/");
            }
            else if (std::isdigit(token) || ((token == '.') && (std::isdigit(input[currCol])))){
                num += token;
            }
            else if (std::isdigit(token) || ((token == '.') && (!std::isdigit(input[currCol])))){
                num += token;
                addToken(Number, num);
            }
            else if (token == '\n'){
                currRow ++;
            }
            else{
                throw Token(Error, token, currRow, currCol);
            }
        }
        addToken(END, "END");
    // }
}

// void Lexer::printTokens(){
    
// }

Token::Token(TokenType type, const std::string &text, int row, int column){
    type   = type;
    this->text   = text;
    row    = row;
    column = column;
}