#include <vector>
#include <memory>

#include "AST.h"

class Block {
private:
    std::vector<std::shared_ptr<Statement>> statements;

public:
    Block();
    virtual ~Block();

    virtual void run();
};

class Statement {
public:
    Statement();
    virtual ~Statement();

    static Block parse_block(const std::vector<Token> &tokens, 
                             std::vector<Token>::const_iterator &head);
    virtual void run();
    virtual bool syntax_valid() = 0;
};

class Expression : public Statement {
public:
    Expression();
    virtual ~Expression();

    std::shared_ptr<AST> expr;

    virtual bool syntax_valid() override;
};

class IfStatement : public Statement {
public:
    IfStatement();
    virtual ~IfStatement();

    Expression condExpr;
    Block blockA;
    Block blockB;

    virtual bool syntax_valid();
};

class ElseStatement : public Statement {
public:
    ElseStatement();
    virtual ~ElseStatement();

    Block eBlock;

    virtual bool syntax_valid();
};

class WhileStatement : public Statement {
public:
    WhileStatement();
    virtual ~WhileStatement();

    Expression condExpr;
    Block wBlock;

    virtual bool syntax_valid();
};

class PrintStatement : public Statement {
public:
    PrintStatement();
    virtual ~PrintStatement();

    Expression printee;

    virtual bool syntax_valid();
};