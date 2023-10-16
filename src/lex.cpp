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

const std::vector<Token>& Lexer::getTokens() const{
    return tokens;
}

Token::Token(TokenType type, const std::string &text, int row, int column): chr(text){
    this->type   = type;
    this->row    = row;
    this->column = column;
}

void Lexer::tokenize(std::istream& input){
    // num stores number, e.g. 8.8, 134.45
    // str is used mainly for throwing exception since Token() 
    // only accepts string in the 2nd parameter
    // period2 keeps track of where the 2nd period is when there
    // is error such as 8..9
    // temp keeps track of how many periods we have encountered
    // token keeps track of the current character from input
    std::string num, str; 
    int period2, temp(0);
    char token;
    
    while (input.get(token)){
        // For each character in the input, we check if it is one
        // of the operators we recognize or if it is a valid number
        // If it is not a valid number or not a operator that we 
        // know of, we throw an error
        currCol ++;
        if (token == '\n'){
            currRow ++;
            currCol = 0;
        }
        else if (std::isspace(token)){}
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
                // if we encounter a period, we increment temp
                // and store the position of 2nd period (where
                // the error location should be) in period2
                if (temp==0){
                    temp++;
                }
                else if (temp==1){
                    period2 = currCol;
                    temp++;
                }
            }
            if (std::isdigit(input.peek()) || (input.peek() == '.')){
                // if the next character is a number or a period
                // we keep reading from input
                continue;
            }
            else{
                // if the number is completely read, we now check if 
                // the number is valid by counting the number of period
                // inside the number
                int n = std::count(num.begin(), num.end(), '.');
                if (n == 0){
                    // if there is no period, just add the number
                    addToken(TokenType::Number, num, currRow, currCol-(num.length()-1));
                    num = "";
                }
                else if (n == 1){
                    // if there is one period, there are 2 instances when 
                    // the number would be invalid e.g.
                    // 1) .9, error position = 1
                    // 2) 9., error position = 3
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
                    // if there are more than 1 periods, the number is definitely invalid
                    // 1) ..9, error position = 1
                    // 2) 9..9, error position = 3
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
                    }
                }
                temp = 0;
            }
        }
        else{
            str += token;
            throw Token(TokenType::Error, str, currRow, currCol);
            str = "";
        }
    }
    currCol ++;
    addToken(TokenType::END, "END", currRow, currCol);
    return;
}

int main() {
    try{
        Lexer lexer;
        lexer.tokenize(std::cin);

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