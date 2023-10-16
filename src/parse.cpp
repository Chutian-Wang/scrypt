#include <iostream>
#include <iomanip>
#include <sstream>
#include "lib/Lexer.h"
#include "lib/AST.h"
#include "lib/Errors.h"

int main() {
    static AST* parser;
    try{
        Lexer lexer;
        lexer.tokenize(std::cin);

        auto tokens = lexer.get_tokens();
        parser = AST::parse(tokens);
        std::ostringstream oss;
        parser->get_infix(oss);
        std::cout << oss.str() << '\n';
    }
    catch(const SyntaxError& err) {
        std::cout << err.what();
        exit(SYNTAX_ERR);
    }
    catch(const UnexpTokError& err) {
        std::cout << err.what();
        exit(UNEXP_TOK);
    }
    
    try {
        std::cout << parser->eval() << std::endl;
        delete parser;
    }
    catch(const DivByZero& err) {
        std::cout << err.what();
        exit(DIV_BY_ZERO);
    }

    return 0;
}