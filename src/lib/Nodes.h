#ifndef NODES_H
#define NODES_H
#include "AST.h"
#include <string>
#include <vector>

class Number: public AST {
private:
    Token tok;
    
    double val;

public:
    Number(const Token& tok);
    virtual ~Number();

    virtual const Token&    get_token() const;
    virtual double          eval()      const;
    virtual bool            is_legal()  const;
    virtual std::string     get_infix() const;
};

class Operator: public AST {
private:
    Token tok;
    bool validated;
    bool legal;

    std::vector<AST*> operands;

public:
    Operator(const Token& tok);
    virtual ~Operator();

    void add_oprand(std::vector<AST*> nodes);
    void add_oprand(AST* node);

    virtual const Token&    get_token() const;
    virtual double          eval()      const;
    virtual bool            is_legal()  const;
    virtual std::string     get_infix() const;
};
#endif