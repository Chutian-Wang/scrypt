#ifndef TOKEN_H
#define TOKEN_H

#include <vector>
#include <string>

enum class TokenType {
    Number, 
    Operator,
    Lparen,     // (
    Rparen,     // )
    END,        
    Error      // Any other 
};

class Token{
    public:
        TokenType type;
        std::string text;
        int row;
        int column;

        Token(TokenType type, const std::string &text, int row, int column);
};

#endif //TOKEN_H