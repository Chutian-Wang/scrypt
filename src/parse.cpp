#include <iostream>
#include <iomanip>
#include <sstream>
#include "lib/Lexer.h"
#include "lib/AST.h"

int main() {
    try{
        Lexer lexer;
        lexer.tokenize(std::cin);

        auto tokens = lexer.get_tokens();
        AST* parser = AST::parse(tokens);
        std::ostringstream oss;
        parser->get_infix(oss);
        std::cout << oss.str() << '\n';
        std::cout << parser->eval() << std::endl;
        delete parser;
    }
    catch(Token error){
        std::cout << "Syntax error on line " << error.row << " column " << error.column << ".\n";
        exit(1);
    }

    return 0;
}