#include <iostream>
#include <iomanip>
#include "lib/Lexer.h"
#include "lib/Errors.h"

int main() {
    try{
        Lexer lexer;
        lexer.tokenize(std::cin);

        std::vector<Token> tokens = lexer.get_tokens();

        for(auto token: tokens){
            std::cout << std::setw(4) << token.row;
            std::cout << std::setw(5) << token.column << "  ";
            std::cout << token.text << std::endl;
        }
    }
    catch(SyntaxError& err) {
        std::cout << err.what();
        exit(SYNTAX_ERR);
    }
    catch(UnexpTokError& err) {
        std::cout << err.what();
        exit(UNEXP_TOK);
    }
    catch(ScryptError& err) {
        std::cout << "Bad ScryptError" << std::endl;
    }

    return 0;
}