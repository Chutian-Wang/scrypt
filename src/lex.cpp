#include "lib/demo_lib.h"
#include <iomanip>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
#include "lib/token.h"
#include "lib/lex.h"

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
                int n = std::count(num.begin(), num.end(), '.');
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

int main() {
    // std::string line;
    
    // Lexer lexer;
    
    
    // std::vector<Token> tokens;
    // tokens = lexer.getTokens();

    // for(auto token: tokens){
    //     std::cout << std::setw(3) << token.row;
    //     std::cout << std::setw(4) << token.column << "  ";
    //     std::cout << token.chr;
    // }

    std::string line;
    
    //while(std::getline(std::cin, line));
    std::getline(std::cin, line);
    std::cout << line << std::endl;
    try{
        Lexer lexer;
        lexer.tokenize(line);

        std::vector<Token> tokens;
        tokens = lexer.getTokens();

        for(auto token: tokens){
            std::cout << std::setw(4) << token.row;
            std::cout << std::setw(5) << token.column << "  ";
            std::cout << token.chr << std::endl;
        }
    }
    catch(Token error){
        std::cout << "Syntax error on line " << error.row << " column " << error.column << ".\n";
        exit(1);
    }

    return 0;
}