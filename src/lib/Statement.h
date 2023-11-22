#include <iostream>
#include <memory>
#include <stack>
#include <vector>

#include "AST.h"
#include "Function.h"

enum struct StatementType {
  DEFAULT,
  EXPRES,
  IF,
  WHILE,
  PRINT,
  FUNCT,
  RETURN
};


// Avoid referencing compiler error
// We might want to rethink our design choices...
class Block;
class Statement;
class Function;

// extern std::map<std::string, FunctStatement> function_map;

// A collection of statements which can be run as a whole
// An entire scrypt program can and should be a block
class Block {
 private:
  std::vector<std::unique_ptr<Statement>> statements;

 public:
  Block();
  ~Block();

  // This is the top level block parser.
  static std::unique_ptr<Block> parse_block(const std::vector<Token>& tokens);
  // This function will set head to one past the last token
  // read unless the last token is END, in which case it will
  // be set to END.
  static std::unique_ptr<Block> parse_block(
      std::vector<Token>::const_iterator& head);

  void add_statement(std::unique_ptr<Statement> statement);
  void run(std::shared_ptr<Function> currentScope);
  void print(std::ostream& os, int depth = 0) const;
};

// Statement virtual base class
class Statement {
 protected:
  std::stack<std::shared_ptr<Function>> scopeStack;

 public:
  Statement(){};
  Statement(std::stack<std::shared_ptr<Function>> anyScopeStack);
  virtual ~Statement(){};
  StatementType type;

  // This function runs  the statement and will
  // print something if it contains a print statement
  virtual void run(std::shared_ptr<Function> currentScope) = 0;
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

  virtual void run(std::shared_ptr<Function> currentScope);
  virtual void print(std::ostream& os, int depth = 0) const;

  Value eval(std::shared_ptr<Function> currentScope);
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

  virtual void run(std::shared_ptr<Function> currentScope);
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

  virtual void run(std::shared_ptr<Function> currentScope);
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
  virtual void run(std::shared_ptr<Function> currentScope);
  // This function will push the print expression
  // to the passed in os, not this->os
  virtual void print(std::ostream& os, int depth = 0) const;
};

class FunctStatement : public Statement {
 private:
  std::vector<std::shared_ptr<AST>> arguments;
  std::shared_ptr<AST> name;
  std::unique_ptr<Block> function_block;

 public:
  FunctStatement();
  virtual ~FunctStatement();

  virtual void run(std::shared_ptr<Function> currentScope);
  virtual void print(std::ostream& os, int depth = 0) const;

  void set_argument(std::vector<std::shared_ptr<AST>>& arg);
  void set_name(std::shared_ptr<AST>& def);
  void set_function(std::unique_ptr<Block>& block);
};

class ReturnStatement : public Statement {
 private:
  std::unique_ptr<Expression> ret;

 public:
  ReturnStatement();
  virtual ~ReturnStatement();

  virtual void run(std::shared_ptr<Function> currentScope);
  virtual void print(std::ostream& os, int depth = 0) const;

  void set_return(std::unique_ptr<Expression>& value);
};