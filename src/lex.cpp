#include "lib/token.h"
#include "lib/lex.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cctype>

Lexer::Lexer() {
    currRow = 1;
    currCol = 0;
}

Lexer::Lexer(const std::string &input) {
    currRow = 1;
    currCol = 0;
    this->tokenize(input);
}

void Lexer::addToken(TokenType type, const std::string &token){
    tokens.push_back(Token(type, token, currRow, currCol));
}

void Lexer::tokenize(const std::string& expr){
    std::string num;

    for (auto &token : expr){
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
        else if (std::isdigit(token) || (token == '.')){
            num += token;
            if (std::isdigit(expr[currCol]) || (expr[currCol] == '.')){continue;}
            else{
                int n = count(num.begin(), num.end(), '.');
                if (n == 0){
                    addToken(TokenType::Number, num);
                    num = "";
                }
                else if (n == 1){
                    size_t idx = num.find('.');
                    if ((idx == 0) || (idx == num.length()-1)){
                        std::string str;
                        str += token;
                        throw Token(TokenType::Error, str, currRow, currCol);
                    }
                    else{
                        addToken(TokenType::Number, num);
                        num = "";
                    }
                }
                else{
                    std::string str;
                    str += token;
                    throw Token(TokenType::Error, str, currRow, currCol);
                }
            }
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

const std::vector<Token>& Lexer::getTokens() const{
    return tokens;
}

Token::Token(TokenType type, const std::string &text, int row, int column): chr(text){
    this->type   = type;
    this->row    = row;
    this->column = column;
}


