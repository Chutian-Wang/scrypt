#include <iostream>
#include <memory>
#include <vector>

#include "AST.h"

// Avoid referencing compiler error
// We might want to rethink our design choices...
class Block;
class Statement;

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
      std::vector<Token>::const_iterator& head);

  void add_statement(std::unique_ptr<Statement>);
  void run();
  void print(std::ostream& os, int depth = 0) const;
};

// Statement virtual base class
class Statement {
 public:
  Statement(){};
  virtual ~Statement(){};

  // This function runs  the statement and will
  // print something if it contains a print statement
  virtual void run() = 0;
  // This function prints the statement itself
  virtual void print(std::ostream& os, int depth = 0) const = 0;
};

// A single expression as a statement
class Expression : public Statement {
 private:
  std::shared_ptr<AST> expr;

 public:
  Expression(std::shared_ptr<AST> expr);
  ~Expression();

  virtual void run();
  virtual void print(std::ostream& os, int depth = 0) const;

  Value eval();
  void get_infix(std::ostream& os);
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

  virtual void run();
  virtual void print(std::ostream& os, int depth = 0) const;

  void set_cond(std::unique_ptr<Expression>& cond);
  void set_if(std::unique_ptr<Block>& block);
  void set_else(std::unique_ptr<Block>& block);
};

class WhileStatement : public Statement {
 private:
  std::unique_ptr<Expression> condition;
  std::unique_ptr<Block> while_block;

 public:
  WhileStatement();
  virtual ~WhileStatement();

  virtual void run();
  virtual void print(std::ostream& os, int depth = 0) const;

  void set_cond(std::unique_ptr<Expression>& cond);
  void set_while(std::unique_ptr<Block>& block);
};

class PrintStatement : public Statement {
 private:
  std::ostream& os;
  std::unique_ptr<Expression> printee;

 public:
  // The print target ostream must be provided in
  // initializaer list.
  PrintStatement(std::unique_ptr<Expression>& printee, std::ostream& os);
  virtual ~PrintStatement();

  // This function will push printee's evaluated
  // value to this->os
  virtual void run();
  // This function will push the print expression
  // to the passed in os, not this->os
  virtual void print(std::ostream& os, int depth = 0) const;
};