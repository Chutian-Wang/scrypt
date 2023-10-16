#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum struct TokenType {
    NUMBER,     
    OPERATOR,   // +, -, *, /
    LPAREN,     // (
    RPAREN,     // )
    END,        
    ERR         // Any other 
};

class Token{
public:
    TokenType   type;
    std::string text; // what the token is, e.g. "+", used later for printing
    int         row;
    int         column;

    Token();
    Token(TokenType type, const std::string &text, int row, int column);
};

#endif //TOKEN_H