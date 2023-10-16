#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include "Token.h"

// This is the base class for all Nodes.
class AST {
public:
    // Function for parsing top level tree creation,
    // This function calls the explicit version with
    // head = tokens.begin()
    static AST* parse(const std::vector<Token> & tokens);
    static AST* parse(const std::vector<Token> & tokens,
        std::vector<Token>::const_iterator& head);

public:
    virtual ~AST() {};
    // Function to evaluate the subtree:
    virtual const Token&        get_token() const = 0;
    virtual double              eval()      const = 0;
    virtual bool                is_legal()  const = 0;
    virtual std::string         get_infix() const = 0;
};

#endif
