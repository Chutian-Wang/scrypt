#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include "token.h"

class Lexer{
    public:
        //Lexer(const std::string &input);
        
        // Default constructor for empty instance creation
        Lexer();

        // tokenize() is used to tokenize input S expression
        // into tokens, which are then stored in vector tokens
        // These tokens must be stored in **prefix order**,
        // e.g. {(, +, (, *, 1, 2, ), 3, )} parsed from the S-
        // expression (+ (* 1 2) 3) which evaluates to the
        // mathematical expression 1 + 2 * 3
        void tokenize();

        const std::vector<Token>& getTokens() const;

    private:
        std::vector<Token> tokens;
        int                currRow;
        int                currCol;

        // If we must store raw text do it here
        // Each element of this vector contains a line of raw
        // text
        std::vector<std::string> textLines;

        // Helper function
        // Used in tokenize() to add a new Token into the vector tokens 
        // each time a new token is reached
        void addToken(TokenType type, const std::string &token, int row, int col);
};

#endif //LEXER_H
