#include <iostream>
#include <iomanip>
#include "lib/Lexer.h"

int main() {

    Lexer lexer;
    lexer.tokenize(std::cin);

    std::vector<Token> tokens = lexer.get_tokens();

    for(auto token: tokens){
        std::cout << std::setw(4) << token.row;
        std::cout << std::setw(5) << token.column << "  ";
        std::cout << token.text << std::endl;
    }
    
    return 0;
}