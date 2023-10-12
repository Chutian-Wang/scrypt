#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>

class Lexer{
    public:
        Lexer(std::string &input);
        // tokenize() is used to tokenize input S expression into tokens
        // Tokens are then stored in vector tokens
        void               tokenize();
        // getTokens() used to fetch the private member tokens 
        std::vector<Token> getTokens();

    private:
        std::string        &text;
        std::vector<Token> tokens;
        int                currRow;
        int                currCol;

        // Helper function
        // Used in tokenize() to add a new Token into the vector tokens 
        // each time a new token is reached
        void addToken(TokenType type, const std::string &token);
};

#endif //LEXER_H