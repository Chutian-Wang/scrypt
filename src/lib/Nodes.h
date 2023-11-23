#ifndef NODES_H
#define NODES_H
#include <memory>
#include <string>
#include <vector>

#include "AST.h"
#include "Value.h"

class Constant : public AST {
 private:
  Token tok;
  Value val;

 public:
  Constant(const Token &tok);
  virtual ~Constant();

  virtual const Token &get_token() const;
  virtual Value eval(std::map<std::string, Value> &scope);
  virtual Value __eval(std::map<std::string, Value> &scope);
  // Deserted due to depreciation of S expression evaluation
  // virtual bool is_legal() const;
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
  virtual Value eval(std::map<std::string, Value> &scope);
  virtual Value __eval(std::map<std::string, Value> &scope);
  // Deserted due to depreciation of S expression evaluation
  // virtual bool is_legal() const;
  virtual void get_infix(std::ostream &oss) const;
};

class Identifier : public AST {
 private:
  Token tok;

 public:
  Identifier(const Token &tok);
  virtual ~Identifier();

  void assign(Value x, std::map<std::string, Value> &scope);
  bool assigned(std::map<std::string, Value> &scope) const;

  virtual const Token &get_token() const;
  virtual Value eval(std::map<std::string, Value> &scope);
  virtual Value __eval(std::map<std::string, Value> &scope);
  // Deserted due to depreciation of S expression evaluation
  // virtual bool is_legal() const;
  virtual void get_infix(std::ostream &oss) const;
};

class FunctionCall : public AST {
 private:
  std::shared_ptr<AST> lhs;
  std::vector<std::shared_ptr<AST>> value;

 public:
  FunctionCall(std::shared_ptr<AST> node,
               std::vector<std::shared_ptr<AST>> value);
  virtual ~FunctionCall();

  virtual const Token &get_token() const;
  const std::vector<std::shared_ptr<AST>> &get_value() const;
  const std::vector<Value> arg_val(std::map<std::string, Value> &scope) const;
  virtual Value eval(std::map<std::string, Value> &scope);
  virtual Value __eval(std::map<std::string, Value> &scope);
  // Deserted due to depreciation of S expression evaluation
  // virtual bool is_legal() const;
  virtual void get_infix(std::ostream &oss) const;
};

class Array : public AST {
 private:
  Token tok;
  std::shared_ptr<AST> identifier;
  std::shared_ptr<AST> acc_index;
  std::vector<std::shared_ptr<AST>> literals;

 public:
  static Value access(Value arr, const Value &index);

  static Value len(Array array);
  static Value pop(Array array);
  static void push(Value val);

  Array();
  virtual ~Array();

  void add_literal(std::shared_ptr<AST> literal);
  void set_identifier(std::shared_ptr<AST> identifier);
  void set_acc_index(std::shared_ptr<AST> index);
  void set_token(const Token &tok);
  void assign(const Value &val, std::map<std::string, Value> &scope);

  virtual const Token &get_token() const;
  virtual Value eval(std::map<std::string, Value> &scope);
  virtual Value __eval(std::map<std::string, Value> &scope);

  virtual void get_infix(std::ostream &oss) const;
};

#endif
