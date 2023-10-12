#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <vector>
#include <string>

class Lexer{
    public:
        Lexer(std::string &input);
        void tokenize();
        std::vector<Token> getTokens();

    private:
        std::string &text;
        std::vector<Token> tokens;
        int currRow;
        int currCol;

        // Helper function
        void addToken(TokenType type, const std::string &token);
};

#endif //LEXER_H