#ifndef NODES_H
#define NODES_H
#include <memory>
#include <string>
#include <vector>

#include "AST.h"
#include "Value.h"

class Number : public AST {
 private:
  Token tok;
  double val;

 public:
  Number(const Token &tok);
  virtual ~Number();

  virtual const Token &get_token() const;
  virtual Value eval() const;
  virtual Value __eval() const;
  virtual bool is_legal() const;
  virtual void get_infix(std::ostream &oss) const;
};

class Operator : public AST {
 private:
  Token tok;
  bool validated;
  bool legal;

  std::vector<std::shared_ptr<AST>> operands;

 public:
  Operator(const Token &tok);
  virtual ~Operator();

  void add_operand(std::vector<std::shared_ptr<AST>> nodes);
  void add_operand(std::shared_ptr<AST> node);

  virtual const Token &get_token() const;
  virtual Value eval() const;
  virtual Value __eval() const;
  virtual bool is_legal() const;
  virtual void get_infix(std::ostream &oss) const;
};

class Identifier : public AST {
 private:
  Token tok;

 public:
  Identifier(const Token &tok);
  virtual ~Identifier();

  void assign(Value x);
  bool assigned() const;

  virtual const Token &get_token() const;
  virtual Value eval() const;
  virtual Value __eval() const;
  virtual bool is_legal() const;
  virtual void get_infix(std::ostream &oss) const;
};

#endif
