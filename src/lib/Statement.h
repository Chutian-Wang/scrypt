#include <memory>
#include <vector>

#include "AST.h"

class Block {
 private:
  std::vector<std::shared_ptr<Statement>> statements;

 public:
  Block();
  virtual ~Block();

  virtual void run();
  virtual const Statement get_statement();
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
 private:
  std::shared_ptr<AST> expr;

 public:
  Expression();
  virtual ~Expression();

  virtual bool syntax_valid() override;
};

class IfStatement : public Statement {
 private:
  Expression condExpr;
  Block blockA;
  Block blockB;

 public:
  IfStatement();
  virtual ~IfStatement();

  virtual bool syntax_valid();
};

class ElseStatement : public Statement {
 private:
  Block eBlock;

 public:
  ElseStatement();
  virtual ~ElseStatement();

  virtual bool syntax_valid();
};

class WhileStatement : public Statement {
 private:
  Expression condExpr;
  Block wBlock;

 public:
  WhileStatement();
  virtual ~WhileStatement();

  virtual bool syntax_valid();
};

class PrintStatement : public Statement {
 private:
  Expression printee;

 public:
  PrintStatement();
  virtual ~PrintStatement();

  virtual bool syntax_valid();
};