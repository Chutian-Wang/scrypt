#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include "token.h"

class Lexer{
    public:
        Lexer(const std::string &input);
        // Default constructor for empty instance creation
        Lexer();

        // tokenize() is used to tokenize input S expression into tokens
        // Tokens are then stored in vector tokens
        void tokenize(const std::string& expr);

        // getTokens() used to fetch the private member tokens 
        const std::vector<Token>& getTokens();

    private:
        std::vector<Token> tokens;
        int                currRow;
        int                currCol;

        // Helper function
        // Used in tokenize() to add a new Token into the vector tokens 
        // each time a new token is reached
        void addToken(TokenType type, const std::string &token);
};

#endif //LEXER_H
