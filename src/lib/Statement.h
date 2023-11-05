#include <iostream>
#include <memory>
#include <vector>

#include "AST.h"

// A collection of statements which can be run as a whole
// An entire scrypt program can and should be a block
class Block {
 private:
  std::vector<std::unique_ptr<Statement>> statements;

 public:
  Block();
  ~Block();

  static std::unique_ptr<Block> parse_block(
      const std::vector<Token>& tokens,
      std::vector<Token>::const_iterator& head,
      std::vector<std::unique_ptr<Statement>>& statements);

  void add_statement(std::unique_ptr<Statement>);
  void run();
  void print(std::ostream& os);
};

// Statement virtual base class
class Statement {
 public:
  Statement(){};
  virtual ~Statement();

  virtual bool syntax_valid() = 0;
  // This function runs  the statement and will
  // print something if it contains a print statement
  virtual void run() = 0;
  // This function prints the statement itself
  virtual void print(std::ostream) = 0;
};

// A single expression as a statement
class Expression : public Statement {
 private:
  std::shared_ptr<AST> expr;

 public:
  Expression(std::unique_ptr<AST> expr);
  ~Expression();

  virtual bool syntax_valid();
  virtual void run();
  virtual void print(std::ostream& os);
};

class IfStatement : public Statement {
 private:
  std::unique_ptr<Expression> condition;
  std::unique_ptr<Block> if_block;
  // If an else block does not exist this pointer is null
  std::unique_ptr<Block> else_block;

 public:
  IfStatement();
  virtual ~IfStatement();

  virtual bool syntax_valid();
  virtual void run();
  virtual void print(std::ostream& os);

  void set_cond(std::unique_ptr<Expression>);
  void set_if(std::unique_ptr<Block>);
  void set_else(std::unique_ptr<Block>);
};

class WhileStatement : public Statement {
 private:
  std::unique_ptr<Expression> condition;
  std::unique_ptr<Block> while_block;

 public:
  WhileStatement();
  virtual ~WhileStatement();

  virtual bool syntax_valid();
  virtual void run();
  virtual void print(std::ostream& os);

  void set_cond(std::unique_ptr<Expression>);
  void set_while(std::unique_ptr<Block>);
};

class PrintStatement : public Statement {
 private:
  std::ostream& os;
  std::unique_ptr<Expression> printee;

 public:
  PrintStatement(std::unique_ptr<Expression> printee);
  virtual ~PrintStatement();

  virtual bool syntax_valid();
  // This function will push printee's evaluated
  // value to this->os
  virtual void run();
  // This function will push the print expression
  // to the passed in os, not this->os
  virtual void print(std::ostream& os);

  void set_ostream(std::ostream& os);
};