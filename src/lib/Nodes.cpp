#include "Nodes.h"
#include "Token.h"
#include "Errors.h"

// Number implementations ------------------------------------
Number::Number(const Token& tok) {
    this->tok = tok;
    this->val = std::stod(tok.text);
}

Number::~Number() {
    // Nothing on the heap
}

const Token& Number::get_token() const {
    return this->tok;
}

double Number::eval() const {
    return this->val;
}

bool Number::is_legal() const {
    return true;
}

std::string Number::get_infix() const {
    return std::to_string(this->val);
}

// Operator implememtations ----------------------------------
Operator::Operator(const Token& tok) {
    this->tok = tok;
    this->validated = false;
    this->legal = false;
}

Operator::~Operator() {
    for (auto node: this->operands) {
        delete node;
    }
}

void Operator:: add_oprand(std::vector<AST*> nodes) {
    for (auto node: nodes) {
        this->add_oprand(node);
    }
}

void Operator:: add_oprand(AST* node) {
    this->validated = false;
    this->operands.push_back(node);
}

const Token& Operator::get_token() const {
    return this->tok;
}

double Operator::eval() const {
    switch ((this->tok).text[0])
    {
        case ('+'): {
            double ret = 0;
            for (auto node: this->operands) {
                ret += node->eval();
            }
            return ret;
            break;
        }
        case ('-'): {
            double ret = this->operands[0]->eval();
            for (auto node = (this->operands).begin() + 1;
                node < this->operands.end(); node++) {
                ret -= (*node)->eval();
            }
            return ret;
            break;
        }
        case ('*'): {
            double ret = 1;
            for (auto node: this->operands) {
                ret *= node->eval();
            }
            return ret;
            break;
        }
        case ('/'): {
            double ret = this->operands[0]->eval();
            for (auto node = (this->operands).begin() + 1;
                node < this->operands.end(); node++) {
                if ((*node)->eval() == 0. && ret != 0.) {
                    div_by_zero_err();
                }
                ret /= (*node)->eval();
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
    if (this->validated) return this->legal;
    else {
        if (this->operands.size() < 2) return false;
        for (auto node: this->operands) {
            if (!(node->is_legal())) return false;
        }
        return true;
    }
}

std::string Operator::get_infix() const {
    std::string ret = "(";
    for (auto node = this->operands.begin();
        node < this->operands.end(); node++) {
        ret += (*node)->get_infix();
        if (this->operands.end() - node > 1) {
            ret = ret + " " + this->tok.text + " ";
        }
        else {
            ret += ")";
        }
    }
    return ret;
}