#ifndef NODES_H
#define NODES_H
#include <string>
#include <vector>

#include "AST.h"

class Number : public AST {
 private:
  Token tok;

  double val;

 public:
  Number(const Token &tok);
  virtual ~Number();

  virtual const Token &get_token() const;
  virtual double eval() const;
  virtual bool is_legal() const;
  virtual void get_infix(std::ostringstream &oss) const;
};

class Operator : public AST {
 private:
  Token tok;
  bool validated;
  bool legal;

  std::vector<AST *> operands;

 public:
  Operator(const Token &tok);
  virtual ~Operator();

  void add_operand(std::vector<AST *> nodes);
  void add_operand(AST *node);

  virtual const Token &get_token() const;
  virtual double eval() const;
  virtual bool is_legal() const;
  virtual void get_infix(std::ostringstream &oss) const;
};
#endif
