#include "Nodes.h"

#include <cmath>
#include <map>
#include <sstream>

#include "Errors.h"
#include "Token.h"
#include "Value.h"

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

Value Number::eval() const { return this->__eval(); }

Value Number::__eval() const { return this->val; }

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

Value Operator::eval() const {
  // auto old_map = symbols;
  try {
    return this->__eval();
  } catch (const ScryptRuntimeError &err) {
    // Restore variables
    // symbols = old_map;
    throw;
  }
}

Value Operator::__eval() const {
  std::string str = (this->tok).text;
  if (str == "+") {
    Value ret(0.0);
    for (auto node : this->operands) {
      ret._double += node->__eval()._double;
    }
    return ret;
  } else if (str == "-") {
    Value ret(this->operands[0]->__eval()._double);
    for (auto node = (this->operands).begin() + 1; node < this->operands.end();
         node++) {
      ret._double -= (*node)->__eval()._double;
    }
    return ret;
  } else if (str == "*") {
    Value ret(1.0);
    for (auto node : this->operands) {
      ret._double *= node->__eval()._double;
    }
    return ret;
  } else if (str == "/") {
    Value ret(this->operands[0]->__eval()._double);
    for (auto node = (this->operands).begin() + 1; node < this->operands.end();
         node++) {
      if ((*node)->__eval()._double == 0.) {
        throw DivByZero();
      }
      ret._double /= (*node)->__eval()._double;
    }
    return ret;
  } else if (str == "=") {
    // get rhs value
    Value ret((*((this->operands).end() - 1))->__eval()._double);
    for (auto node = ((this->operands).end() - 2);
         node >= ((this->operands).begin()); node--) {
      ((Identifier *)(node->get()))->assign(ret);
    }
    return ret;
  } else if (str == "==") {
    if ((this->operands[0]->__eval().type == ValueType::BOOL) && (this->operands[1]->__eval().type == ValueType::BOOL)) {
      Value ret((this->operands[0]->__eval()._bool) == ((this->operands[1]->__eval()._bool)));
      return ret;
    } else if ((this->operands[0]->__eval().type == ValueType::DOUBLE) && (this->operands[1]->__eval().type == ValueType::DOUBLE)) {
      Value ret((this->operands[0]->__eval()._double) == ((this->operands[1]->__eval()._double)));
      return ret;
    } else {
      throw InvalidOperand();
    }
  } else if (str == "!=") {
    if ((this->operands[0]->__eval().type == ValueType::BOOL) && (this->operands[1]->__eval().type == ValueType::BOOL)) {
      Value ret((this->operands[0]->__eval()._bool) != ((this->operands[1]->__eval()._bool)));
      return ret;
    } else if ((this->operands[0]->__eval().type == ValueType::DOUBLE) && (this->operands[1]->__eval().type == ValueType::DOUBLE)) {
      Value ret((this->operands[0]->__eval()._double) != ((this->operands[1]->__eval()._double)));
      return ret;
    } else {
      throw InvalidOperand();
    }
  } else if (str == "<") {
    if ((this->operands[0]->__eval().type == ValueType::DOUBLE) && (this->operands[1]->__eval().type == ValueType::DOUBLE)) {
      Value ret((this->operands[0]->__eval()._double) < ((this->operands[1]->__eval()._double)));
      return ret;
    } else {
      throw InvalidOperand();
    }
  } else if (str == "<=") {
    if ((this->operands[0]->__eval().type == ValueType::DOUBLE) && (this->operands[1]->__eval().type == ValueType::DOUBLE)) {
      Value ret((this->operands[0]->__eval()._double) <= ((this->operands[1]->__eval()._double)));
      return ret;
    } else {
      throw InvalidOperand();
    }
  } else if (str == ">") {
    if ((this->operands[0]->__eval().type == ValueType::DOUBLE) && (this->operands[1]->__eval().type == ValueType::DOUBLE)) {
      Value ret((this->operands[0]->__eval()._double) > ((this->operands[1]->__eval()._double)));
      return ret;
    } else {
      throw InvalidOperand();
    }
  } else if (str == ">=") {
    if ((this->operands[0]->__eval().type == ValueType::DOUBLE) && (this->operands[1]->__eval().type == ValueType::DOUBLE)) {
      Value ret((this->operands[0]->__eval()._double) >= ((this->operands[1]->__eval()._double)));
      return ret;
    } else {
      throw InvalidOperand();
    }
  }
  return Value(0.0);
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

Value Identifier::eval() const { return this->__eval(); }

Value Identifier::__eval() const {
  if (this->assigned()) {
    return symbols[this->tok.text];
  } else {
    throw UnknownIdent(this->tok);
  }
}

bool Identifier::is_legal() const { return this->assigned(); }

void Identifier::assign(Value x) {
  if (x.type == ValueType::BOOL) {
    symbols[this->tok.text] = x._bool;
  }
  if (x.type == ValueType::DOUBLE) {
    symbols[this->tok.text] = x._double;
  }
}

bool Identifier::assigned() const {
  return !(symbols.find(this->tok.text) == symbols.end());
}

void Identifier::get_infix(std::ostream &oss) const { oss << this->tok.text; }
