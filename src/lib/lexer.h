##ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <vector>
#include "token.h"

class Lexer{
    public:
        Lexer(std::istream &input);
        std::vector<Token> tokenize();
        void printTokens();

    private:
        std::istream &input;
        std::vector<Token> tokens;
        int currRow;
        int currCol;

        // Helper function
        void addToken(TokenType type, const std::string &token);
};