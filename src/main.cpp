#include "lib/demo_lib.h"
#include <iomanip>
#include <vector>
#include "lib/token.h"
#include "lib/lex.h"

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
    // while(std::getline(std::cin, line));
    std::getline(std::cin, line);
    try{
        Lexer lexer;
        lexer.tokenize(line);

        std::vector<Token> tokens;
        tokens = lexer.getTokens();

        for(auto token: tokens){
            std::cout << std::setw(3) << token.row;
            std::cout << std::setw(4) << token.column << "  ";
            std::cout << token.chr << std::endl;
        }
    }
    catch(Token error){
        std::cout << "Syntax error on line " << error.row << " column " << error.column << ".\n";
        exit(1);
    }

    return 0;
}