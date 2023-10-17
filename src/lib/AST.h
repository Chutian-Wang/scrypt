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

    virtual ~AST() {};
    
    // Function for parse() to get sub-class tokens
    virtual const Token&        get_token() const = 0;
    // Function for parse to get validity of a node
    virtual bool                is_legal()  const = 0;
    // Function to evaluate the subtree:
    virtual double              eval()      const = 0;
    // This function pushes the subtree's infix form into oss
    virtual void get_infix(std::ostringstream& oss) const = 0;
};

#endif
