#include "Nodes.h"

#include <map>

#include "Errors.h"
#include "Token.h"
#include "Value.h"

// Constant implementations ------------------------------------
Constant::Constant(const Token &tok) {
  this->tok = tok;
  this->val = std::stod(tok.text);
}

Constant::~Constant() {}

const Token &Constant::get_token() const { return this->tok; }

Value Constant::eval() const { return this->__eval(); }

Value Constant::__eval() const { return this->val; }

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

Value Operator::eval() const {
  auto old_map = symbols;
  try {
    return this->__eval();
  } catch (const ScryptRuntimeError &err) {
    // Restore variables
    symbols = old_map;
    throw;
  }
}

Value Operator::__eval() const {
  std::string str = (this->tok).text;

  if (str == "+") {
    Value ret(0.0);
    for (auto node : this->operands) {
      ret += node->__eval();
    }
    return ret;
  } else if (str == "-") {
    Value ret = this->operands[0]->__eval();
    for (auto node = (this->operands).begin() + 1; node < this->operands.end();
         node++) {
      ret -= (*node)->__eval();
    }
    return ret;
  } else if (str == "*") {
    Value ret(1.0);
    for (auto node : this->operands) {
      ret *= node->__eval();
    }
    return ret;
  } else if (str == "/") {
    Value ret(this->operands[0]->__eval()._value._double);
    for (auto node = (this->operands).begin() + 1; node < this->operands.end();
         node++) {
      ret /= (*node)->__eval();
    }
    return ret;
  } else if (str == "%") {
    Value ret(this->operands[0]->__eval()._value._double);
    for (auto node = (this->operands).begin() + 1; node < this->operands.end();
         node++) {
      ret %= (*node)->__eval();
    }
    return ret;
  } else if (str == "=") {
    // Get rhs value
    Value ret = (*((this->operands).end() - 1))->__eval();
    // Assign to lhs'
    for (auto node = ((this->operands).end() - 2);
         node >= ((this->operands).begin()); node--) {
      ((Identifier *)(node->get()))->assign(ret);
    }
    // Return rhs
    return ret;
    // The following operators do not support S expression
    // tree structure.
  } else if (str == "==") {
    return this->operands[0]->__eval() == this->operands[1]->__eval();
  } else if (str == "!=") {
    return this->operands[0]->__eval() != this->operands[1]->__eval();
  } else if (str == "<") {
    return this->operands[0]->__eval() < this->operands[1]->__eval();
  } else if (str == "<=") {
    return this->operands[0]->__eval() <= this->operands[1]->__eval();
  } else if (str == ">") {
    return this->operands[0]->__eval() > this->operands[1]->__eval();
  } else if (str == ">=") {
    return this->operands[0]->__eval() >= this->operands[1]->__eval();
  } else if (str == "&") {
    return this->operands[0]->__eval() & this->operands[1]->__eval();
  } else if (str == "^") {
    return this->operands[0]->__eval() ^ this->operands[1]->__eval();
  } else if (str == "|") {
    return this->operands[0]->__eval() | this->operands[1]->__eval();
  }
  // If execution reaches here something has gone seriously wrong...
  return Value();
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

void Operator::get_infix(std::ostream &oss) const {
  oss << '(';
  for (auto node = this->operands.begin(); node < this->operands.end();
       node++) {
    // problem is with constants true/false
    // works with cases like (12+12)
    // test case: (true + 12)
    // std::cout << "node: " <<(*node)->get_token().text << std::endl;
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
    return symbols.at(this->tok.text);
  } else {
    throw UnknownIdent(this->tok);
  }
}

// Deserted due to depreciation of S expression evaluation
// bool Identifier::is_legal() const { return this->assigned(); }

void Identifier::assign(Value x) { symbols[this->tok.text] = x; }

bool Identifier::assigned() const {
  return !(symbols.find(this->tok.text) == symbols.end());
}

void Identifier::get_infix(std::ostream &oss) const { oss << this->tok.text; }
