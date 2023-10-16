#include "Nodes.h"
#include "Token.h"

// Number implementations ------------------------------------
Number::Number(const Token& tok) {
    this->tok = tok;
    try {
        this->val = std::stod(tok.text);
    }
    catch (std::exception& e) {
        // For the code to reach here the lexer
        // must be critically malfunctioning...
        legal = false;
        this->val = 0;
    }
    this->validated = true;
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
    return this->legal;
}

std::string Number::get_infix() const {
    return this->tok.text;
}

// Operator implememtations ----------------------------------
Operator::Operator(const Token& tok) {
    this->tok = tok;
    this->validated = false;
    this->legal = false;
}

Operator::~Operator() {
    for (auto node: this->oprands) {
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
    this->oprands.push_back(node);
}

const Token& Operator::get_token() const {
    return this->tok;
}

double Operator::eval() const {
    switch ((this->tok).text[0])
    {
        case ('+'): {
            double ret = 0;
            for (auto node: this->oprands) {
                ret += node->eval();
            }
            return ret;
            break;
        }
        case ('-'): {
            double ret = this->oprands[0]->eval();
            for (auto node = (this->oprands).begin() + 1;
                node < this->oprands.end(); node++) {
                ret -= (*node)->eval();
            }
            return ret;
            break;
        }
        case ('*'): {
            double ret = 1;
            for (auto node: this->oprands) {
                ret *= node->eval();
            }
            return ret;
            break;
        }
        case ('/'): {
            double ret = this->oprands[0]->eval();
            for (auto node = (this->oprands).begin() + 1;
                node < this->oprands.end(); node++) {
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
        for (auto node: this->oprands) {
            if (!(node->is_legal())) return false;
        }
        return true;
    }
}

std::string Operator::get_infix() const {
    std::string ret = "(" + this->tok.text + " ";
    for (auto node = this->oprands.begin();
        node < this->oprands.end(); node++) {
        ret += (*node)->get_infix();
        if (this->oprands.end() - node > 1) {
            ret += " ";
        }
        else {
            ret += ")";
        }
    }
    return ret;
}