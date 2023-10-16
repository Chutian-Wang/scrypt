#include <iostream>
#include <iomanip>
#include "lib/Lexer.h"
#include "lib/AST.h"

int main() {
    try{
        Lexer lexer;
        lexer.tokenize(std::cin);

        auto tokens = lexer.get_tokens();
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