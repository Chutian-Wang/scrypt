#include "Nodes.h"

#include <cmath>
#include <map>
#include <sstream>

#include "Errors.h"
#include "Token.h"

extern std::map<std::string, double> symbols;

// Number implementations ------------------------------------
Number::Number(const Token &tok) {
  this->tok = tok;
  this->val = std::stod(tok.text);
}

Number::~Number() {
  // Nothing on the heap
}

const Token &Number::get_token() const { return this->tok; }

double Number::eval() const { return this->__eval(); }

double Number::__eval() const { return this->val; }

bool Number::is_legal() const { return true; }

void Number::get_infix(std::ostream &oss) const { oss << this->val; }

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

double Operator::eval() const {
  auto old_map = symbols;
  try {
    return this->__eval();
  } catch (const ScryptRuntimeError &err) {
    // Restore variables
    symbols = old_map;
    throw;
  }
}

double Operator::__eval() const {
  switch ((this->tok).text[0]) {
    case ('+'): {
      double ret = 0;
      for (auto node : this->operands) {
        ret += node->__eval();
      }
      return ret;
      break;
    }
    case ('-'): {
      double ret = this->operands[0]->__eval();
      for (auto node = (this->operands).begin() + 1;
           node < this->operands.end(); node++) {
        ret -= (*node)->__eval();
      }
      return ret;
      break;
    }
    case ('*'): {
      double ret = 1;
      for (auto node : this->operands) {
        ret *= node->__eval();
      }
      return ret;
      break;
    }
    case ('/'): {
      double ret = this->operands[0]->__eval();
      for (auto node = (this->operands).begin() + 1;
           node < this->operands.end(); node++) {
        if ((*node)->__eval() == 0.) {
          throw DivByZero();
        }
        ret /= (*node)->__eval();
      }
      return ret;
      break;
    }
    case ('='): {
      // get rhs value
      double ret = (*((this->operands).end() - 1))->__eval();
      for (auto node = ((this->operands).end() - 2);
           node >= ((this->operands).begin()); node--) {
        ((Identifier *)(node->get()))->assign(ret);
      }
      return ret;
      break;
    }
    default: {
      return 0;
      break;
    }
  }
}

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

double Identifier::eval() const { return this->__eval(); }

double Identifier::__eval() const {
  if (this->assigned()) {
    return symbols[this->tok.text];
  } else {
    throw UnknownIdent(this->tok);
  }
}

bool Identifier::is_legal() const { return this->assigned(); }

void Identifier::assign(double x) { symbols[this->tok.text] = x; }

bool Identifier::assigned() const {
  return !(symbols.find(this->tok.text) == symbols.end());
}

void Identifier::get_infix(std::ostream &oss) const { oss << this->tok.text; }
