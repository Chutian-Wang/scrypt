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

// Lexer::Lexer(const std::string &input) {
//    currRow = 1;
//    currCol = 0;
//    this->tokenize();
//}

void Lexer::addToken(TokenType type, const std::string &token, int row, int col){
    tokens.push_back(Token(type, token, row, col));
}

void Lexer::tokenize(){
    std::string num = ""; 
    std::string str;
    char character;
    int pcount = 0;
    //iteration part
    while(std::cin.get(character)){
        currCol++;

        if (character == EOF){
            currRow++;
            currCol=1;
            addToken(TokenType::END, "END", currRow, currCol);
            break;
        }
        if (character == '\n'){
            currRow++;
            currCol=0;
        }
        else if (std::isspace(character)){
            continue;
        }
        else if (character == '('){
            addToken(TokenType::Lparen, "(", currRow, currCol);
        }
        else if (character == ')'){
            addToken(TokenType::Rparen, ")", currRow, currCol);
        }
        else if (character == '+'){
            addToken(TokenType::Operator, "+", currRow, currCol);
        }
        else if (character == '-'){
            addToken(TokenType::Operator, "-", currRow, currCol);
        }
        else if (character == '*'){
            addToken(TokenType::Operator, "*", currRow, currCol);
        }
        else if (character == '/'){
            addToken(TokenType::Operator, "/", currRow, currCol);
        }
        else if (std::isdigit(character) || (character  == '.')){
            num += character;
            currCol++;
            char n;
            n = std::cin.peek();
            while((!std::isspace(n) && std::isdigit(n)) || n == '.'){
                char next; 
                std::cin.get(next);
                num += next;
                currCol++;
                if (next == '.'){
                    pcount++;
                }
                if (std::cin.eof()){
                    break;
                }
                if (next == '\n'){
                    currRow++;
                }
                std::cin.get(next);
            }
            //checking if num is valid
            if (!num.empty()) {
                if (pcount == 0) {
                    addToken(TokenType::Number, num, currRow, currCol - num.size());
                    num = "";
                } 
            }
            else if (pcount == 1) {
                size_t idx = num.find('.');
                if (idx == 0 || idx == num.length()-1){
                    str += character;
                    currCol = 1;
                    throw Token(TokenType::Error, str, currRow, currCol - num.size());
                    str = "";
                }
                else{
                    addToken(TokenType::Number, num, currRow, currCol - num.size());
                    num = "";
                }
            }
            else if (pcount > 1){
                throw Token(TokenType::Error, num, currRow, currCol - num.size());
            }
        }
        else{
            str += character;
            throw Token(TokenType::Error, str, currRow, currCol);
            str = "";
        }
    }
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

        std::vector<Token> tokens;
        tokens = lexer.getTokens();

        for(auto &token: tokens){
            std::cout << std::setw(4) << token.row;
            std::cout << std::setw(5) << token.column << "  ";
            std::cout << token.chr << std::endl;
        }
    }
    catch(const Token& error){
        std::cout << "Syntax error on line " << error.row << " column " << error.column << ".\n";
        exit(1);
    }

    return 0;
}