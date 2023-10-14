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

void Lexer::addToken(TokenType type, const std::string &token, int row, int col){
    tokens.push_back(Token(type, token, row, col));
}

void Lexer::tokenize(){
    std::string num, line, str;
    std::getline(std::cin, line);
    int period2, temp(0);
    
    while(!std::cin.eof()){
        for (auto &token : line){
            currCol ++;

            if (std::isspace(token)){}
            else if (token == '('){
                addToken(TokenType::Lparen, "(", currRow, currCol);
            }
            else if (token == ')'){
                addToken(TokenType::Rparen, ")", currRow, currCol);
            }
            else if (token == '+'){
                addToken(TokenType::Operator, "+", currRow, currCol);
            }
            else if (token == '-'){
                addToken(TokenType::Operator, "-", currRow, currCol);
            }
            else if (token == '*'){
                addToken(TokenType::Operator, "*", currRow, currCol);
            }
            else if (token == '/'){
                addToken(TokenType::Operator, "/", currRow, currCol);
            }
            else if (std::isdigit(token) || (token == '.')){
                num += token;
                if (token == '.'){
                    if (temp==0){
                        temp++;
                    }
                    else if (temp==1){
                        period2 = currCol;
                        temp++;
                    }
                }
                if (std::isdigit(line[currCol]) || (line[currCol] == '.')){continue;}
                else{
                    int n = std::count(num.begin(), num.end(), '.');
                    if (n == 0){
                        addToken(TokenType::Number, num, currRow, currCol-(num.length()-1));
                        num = "";
                    }
                    else if (n == 1){
                        size_t idx = num.find('.');
                        if (idx == 0){
                            str += token;
                            throw Token(TokenType::Error, str, currRow, currCol-(num.length()-1));
                            str = "";
                        }
                        else if (idx == num.length()-1){
                            str += token;
                            throw Token(TokenType::Error, str, currRow, currCol+1);
                            str = "";
                        }
                        else{
                            addToken(TokenType::Number, num, currRow, currCol-(num.length()-1));
                            num = "";
                        }
                    }
                    else{
                        size_t idx = num.find('.');
                        if (idx == 0){
                            str += token;
                            throw Token(TokenType::Error, str, currRow, currCol-(num.length()-1));
                            str = "";
                        }
                        else{
                            str += token;
                            throw Token(TokenType::Error, str, currRow, period2);
                            str = "";
                            temp = 0;
                        }
                    }
                }
            }
            // else{
            //     str += token;
            //     throw Token(TokenType::Error, str, currRow, currCol);
            //     str = "";
            // }
        }
        if (std::cin.eof()){
            currCol++;
            addToken(TokenType::END, "END", currRow, currCol);
            break;
        }
        std::getline(std::cin, line);
        currRow++;
        currCol=0;
    }
    currCol++;
    addToken(TokenType::END, "END", currRow, currCol);
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
    try{
        Lexer lexer;
        lexer.tokenize();

        std::vector<Token> tokens = lexer.getTokens();

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