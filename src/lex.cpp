#include <iostream>
#include <iomanip>
#include "lib/Lexer.h"

int main() {
    try{
        Lexer lexer;
        lexer.tokenize(std::cin);

        std::vector<Token> tokens = lexer.getTokens();

        for(auto token: tokens){
            std::cout << std::setw(4) << token.row;
            std::cout << std::setw(5) << token.column << "  ";
            std::cout << token.text << std::endl;
        }
    }
    catch(Token error){
        std::cout << "Syntax error on line " << error.row << " column " << error.column << ".\n";
        exit(1);
    }

    return 0;
}