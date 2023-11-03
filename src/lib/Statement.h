#include <memory>
#include <vector>

#include "AST.h"

class Block {
 private:
  std::vector<std::shared_ptr<Statement>> statements;

 public:
  Block();
  virtual ~Block();

  static void run();
};

class Statement {
 public:
  Statement();
  virtual ~Statement();

  static Block parse_block(const std::vector<Token> &tokens,
                           std::vector<Token>::const_iterator &head);
  virtual bool syntaxValid() = 0;
};

class Expression : public Statement {
 public:
  Expression();
  virtual ~Expression();

  std::shared_ptr<AST> expr;

  virtual bool syntaxValid() override;
};

class IfStatement : public Statement {
 public:
  IfStatement();
  virtual ~IfStatement();

  Expression condExpr;
  Block blockA;
  Block blockB;

  virtual bool syntaxValid();
};

class ElseStatement : public Statement {
 public:
  ElseStatement();
  virtual ~ElseStatement();

  Block eBlock;

  virtual bool syntaxValid();
};

class WhileStatement : public Statement {
 public:
  WhileStatement();
  virtual ~WhileStatement();

  Expression condExpr;
  Block wBlock;

  virtual bool syntaxValid();
};

class PrintStatement : public Statement {
 public:
  PrintStatement();
  virtual ~PrintStatement();

  Expression printee;

  virtual bool syntaxValid();
};