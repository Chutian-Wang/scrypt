#include <iostream>
#include <iomanip>
#include "lib/Lexer.h"
#include "lib/AST.h"

int main() {
    try{
        Lexer lexer;
        lexer.tokenize(std::cin);
        auto tokens = lexer.get_tokens();
        for(auto token: tokens){
            std::cout << std::setw(4) << token.row;
            std::cout << std::setw(5) << token.column << "  ";
            std::cout << token.text << std::endl;
        }
        AST* parser = AST::parse(tokens);
        std::cout << parser->get_infix() << '\n';
        std::cout << parser->eval() << std::endl;
    }
    catch(Token error){
        std::cout << "Syntax error on line " << error.row << " column " << error.column << ".\n";
        exit(1);
    }

    return 0;
}