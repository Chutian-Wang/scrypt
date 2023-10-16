#include <iostream>
#include <iomanip>
#include <sstream>
#include "lib/Lexer.h"
#include "lib/AST.h"
#include "lib/Errors.h"

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
    catch(SyntaxError& err) {
        std::cout << err.what();
        exit(SYNTAX_ERR);
    }
    catch(UnexpTokError& err) {
        std::cout << err.what();
        exit(UNEXP_TOK);
    }
    catch(DivByZero& err) {
        std::cout << err.what();
        exit(DIV_BY_ZERO);
    }

    return 0;
}