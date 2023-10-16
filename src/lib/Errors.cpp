#include "Errors.h"
#include "Token.h"
#include <iostream>

void syntax_err(Token tok) {
    std::cerr <<
        "Syntax error on line " <<
        std::to_string(tok.row) <<
        " column " <<
        std::to_string(tok.column) <<
        '.' << std::endl;
    exit(1);
}

void unexp_tok_err(Token tok) {
    std::cerr <<
        "Unexpected token at line " <<
        std::to_string(tok.row) <<
        " column " <<
        std::to_string(tok.column) <<
        ": " << tok.text << std::endl;
    exit(2);
}

void exp_tok_err(Token tok) {
    std::cerr <<
        "Expecting token at line " <<
        std::to_string(tok.row) <<
        " column " <<
        std::to_string(tok.column) <<
        ": " << tok.text << std::endl;
    exit(2);
}

void div_by_zero_err() {
    std::cerr << "Runtime error: division by zero." << std::endl;
    exit(3);
}