#include "Nodes.h"

#include <cmath>
#include <map>
#include <sstream>
#include <string>

#include "Errors.h"
#include "Function.h"
#include "Token.h"
#include "Value.h"

// Constant implementations ------------------------------------
Constant::Constant(const Token &tok) {
  this->tok = tok;
  if (tok.type == TokenType::NUMBER) {
    this->val = std::stod(tok.text);
  }
  if (tok.type == TokenType::BOOL) {
    std::istringstream is(tok.text);
    bool b;
    is >> std::boolalpha >> b;
    this->val = b;
  }
  if (tok.type == TokenType::null) {
    this->val = nullptr;
  }
}

Constant::~Constant() {}

const Token &Constant::get_token() const { return this->tok; }

Value Constant::eval(std::map<std::string, Value> &scope) const {
  return this->__eval(scope);
}

Value Constant::__eval(std::map<std::string, Value> &) const {
  return this->val;
}

// Deserted due to depreciation of S expression evaluation
// bool Constant::is_legal() const { return true; }

void Constant::get_infix(std::ostream &oss) const { oss << this->val; }

// Operator implememtations ----------------------------------
Operator::Operator(const Token &tok) {
  this->tok = tok;
  this->validated = false;
  this->legal = false;
}

Operator::~Operator() {
  // smart pointer takes care...
}

void Operator::add_operand(std::vector<std::shared_ptr<AST>> nodes) {
  for (auto node : nodes) {
    this->add_operand(node);
  }
}

void Operator::add_operand(std::shared_ptr<AST> node) {
  this->validated = false;
  this->operands.push_back(node);
}

const Token &Operator::get_token() const { return this->tok; }

Value Operator::eval(std::map<std::string, Value> &scope) const {
  auto old_map = scope;
  try {
    return this->__eval(scope);
  } catch (const ScryptRuntimeError &err) {
    // Restore variables
    scope = old_map;
    throw;
  }
}

Value Operator::__eval(std::map<std::string, Value> &scope) const {
  std::string str = (this->tok).text;

  if (str == "+") {
    Value ret(0.0);
    for (auto node : this->operands) {
      ret += node->__eval(scope);
    }
    return ret;
  } else if (str == "-") {
    Value ret = this->operands[0]->__eval(scope);
    for (auto node = (this->operands).begin() + 1; node < this->operands.end();
         node++) {
      ret -= (*node)->__eval(scope);
    }
    return ret;
  } else if (str == "*") {
    Value ret(1.0);
    for (auto node : this->operands) {
      ret *= node->__eval(scope);
    }
    return ret;
  } else if (str == "/") {
    Value ret = this->operands[0]->__eval(scope);
    for (auto node = (this->operands).begin() + 1; node < this->operands.end();
         node++) {
      ret /= (*node)->__eval(scope);
    }
    return ret;
  } else if (str == "%") {
    Value ret = this->operands[0]->__eval(scope);
    for (auto node = (this->operands).begin() + 1; node < this->operands.end();
         node++) {
      ret %= (*node)->__eval(scope);
    }
    return ret;
  } else if (str == "=") {
    if ((this->operands[0]->get_token()).type != TokenType::IDENTIFIER) {
      throw InvalidAssignee();
    }
    // Get rhs value
    Value ret = (*((this->operands).end() - 1))->__eval(scope);
    // Assign to lhs'
    for (auto node = ((this->operands).end() - 2);
         node >= ((this->operands).begin()); node--) {
      ((Identifier *)(node->get()))->assign(ret, scope);
    }
    // Return rhs
    return ret;
    // The following operators do not support S expression
    // tree structure.
  } else if (str == "==") {
    return this->operands[0]->__eval(scope) == this->operands[1]->__eval(scope);
  } else if (str == "!=") {
    return this->operands[0]->__eval(scope) != this->operands[1]->__eval(scope);
  } else if (str == "<") {
    return this->operands[0]->__eval(scope) < this->operands[1]->__eval(scope);
  } else if (str == "<=") {
    return this->operands[0]->__eval(scope) <= this->operands[1]->__eval(scope);
  } else if (str == ">") {
    return this->operands[0]->__eval(scope) > this->operands[1]->__eval(scope);
  } else if (str == ">=") {
    return this->operands[0]->__eval(scope) >= this->operands[1]->__eval(scope);
  } else if (str == "&") {
    return this->operands[0]->__eval(scope) & this->operands[1]->__eval(scope);
  } else if (str == "^") {
    return this->operands[0]->__eval(scope) ^ this->operands[1]->__eval(scope);
  } else if (str == "|") {
    return this->operands[0]->__eval(scope) | this->operands[1]->__eval(scope);
  }
  // If execution reaches here something has gone seriously wrong...
  return Value();
}

void Operator::get_infix(std::ostream &oss) const {
  oss << '(';
  for (auto node = this->operands.begin(); node < this->operands.end();
       node++) {
    (*node)->get_infix(oss);
    if (this->operands.end() - node > 1) {
      oss << ' ' << (this->tok.text) << ' ';
    } else {
      oss << ')';
    }
  }
}

// Identifier implememtations ----------------------------------
Identifier::Identifier(const Token &tok) { this->tok = tok; }

Identifier::~Identifier() {
  // Nothing on the heap
}

const Token &Identifier::get_token() const { return this->tok; }

Value Identifier::eval(std::map<std::string, Value> &scope) const {
  return this->__eval(scope);
}

Value Identifier::__eval(std::map<std::string, Value> &scope) const {
  if (this->assigned(scope)) {
    return scope.at(this->tok.text);
  } else {
    throw UnknownIdent(this->tok);
  }
}

// Deserted due to depreciation of S expression evaluation
// bool Identifier::is_legal() const { return this->assigned(); }

void Identifier::assign(Value x, std::map<std::string, Value> &scope) {
  scope[this->tok.text] = x;
}

bool Identifier::assigned(std::map<std::string, Value> &scope) const {
  return !(scope.find(this->tok.text) == scope.end());
}

void Identifier::get_infix(std::ostream &oss) const { oss << this->tok.text; }

// FunctionCall implememtations ----------------------------------
FunctionCall::FunctionCall(std::shared_ptr<AST> node,
                           std::vector<std::shared_ptr<AST>> value) {
  this->lhs = node;
  this->value = value;
}

FunctionCall::~FunctionCall() {
  // Nothing on the heap
}

const Token &FunctionCall::get_token() const { return lhs->get_token(); }

Value FunctionCall::eval(std::map<std::string, Value> &scope) const {
  auto funct = lhs->__eval(scope);
  if (funct.type != ValueType::FUNCTION) {
    throw NotAFunction();
  }
  return this->__eval(scope);
}

Value FunctionCall::__eval(std::map<std::string, Value> &scope) const {
  Value funct = scope[this->get_token().text];
  FnPtr fnptr = std::get<FnPtr>(funct._value);
  if (fnptr->get_args().size() != value.size()) {
    throw IncorrentArg();
  }
  std::map<std::string, Value> new_scope = fnptr->get_scope();
  std::vector<Value> arg_eval = arg_val(scope);
  for (size_t i = 0; i < arg_eval.size(); ++i) {
    std::string arg = fnptr->get_args()[i];
    new_scope[arg] = arg_eval[i];
  }
  Value result = fnptr->run_code(new_scope);
  return result;
}

const std::vector<Value> FunctionCall::arg_val(
    std::map<std::string, Value> &scope) const {
  // evaluate arguments
  std::vector<Value> arg_eval;
  for (auto val : this->value) {
    arg_eval.push_back(val->eval(scope));
  }
  return arg_eval;
}

void FunctionCall::get_infix(std::ostream &oss) const {
  oss << this->get_token().text << "(";
  if (value.size() != 0) {
    auto node = this->value.begin();
    do {
      (*node)->get_infix(oss);
      if (node + 1 == value.end()) {
        break;
      }
      oss << ", ";
      node++;
    } while (node != value.end());
  }
  oss << ")";
}

const std::vector<std::shared_ptr<AST>> &FunctionCall::get_value() const {
  return value;
}

// Deserted due to depreciation of S expression evaluation
/*
bool Operator::is_legal() const {
  if (this->validated)
    return true;
  else {
    if (this->operands.size() < 2) return false;
    if ((this->tok).text[0] == '=') {
      // Assignment
      // Ensures all but rightmost arguments are identifiers
      for (auto node = this->operands.begin(); node < this->operands.end();
           node++) {
        if (*node != this->operands.back() &&
            (*node)->get_token().type != TokenType::IDENTIFIER) {
          return false;
        }
      }
      // Ensures right most node is legal
      if (!operands.back()->is_legal()) {
        return false;
      }
      return true;
    } else {
      // Regular node
      for (auto node : this->operands) {
        if (!(node->is_legal()) &&
            node->get_token().type != TokenType::IDENTIFIER)
          return false;
      }
      return true;
    }
  }
}
*/