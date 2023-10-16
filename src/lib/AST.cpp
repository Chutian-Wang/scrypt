#include "AST.h"
#include "Nodes.h"
#include "Errors.h"
#include <vector>
#include <stack>
#include <string>

AST* AST::parse(const std::vector<Token> & tokens) {
    // Check if given empty expression
    if (tokens.size() == 1 && tokens[0].type == TokenType::END) {
        unexp_tok_err(tokens[0]);
    };
    // Check if fist token is LRAREN
    if (tokens[0].type != TokenType::LPAREN) {
        unexp_tok_err(tokens[0]);
    }
    // Check if first node is number.
    if (tokens[1].type == TokenType::NUMBER) {
        unexp_tok_err(tokens[1]);
    }
    auto head = tokens.begin();
    auto ret = AST::parse(tokens, head);
    head++;
    // Check if END is reached and nothing is left
    if (tokens.end() - head > 1 or (*head).type != TokenType::END) { 
        delete ret;
        // Something after END
        if (tokens.end() - head > 1){
            unexp_tok_err(*(head));
        }
        // No END
        else {
            exp_tok_err(Token(TokenType::END ,"END", (*head).row, (*head).column + 1));
        }
    }
    return ret;
}

// After call to parse head is set to the last token read.
AST* AST::parse(const std::vector<Token>& tokens,
    std::vector<Token>::const_iterator& head) {

    std::vector<AST*> node_queue;

    while (head < tokens.end() && (*head).type != TokenType::END) {
        head++;
        switch ((*head).type) {
            case (TokenType::LPAREN): {
                // when parse returns, head is set to one
                // token past the matching RPARAN in the
                // next cycle.
                node_queue.push_back(parse(tokens, head));
                break;
            }
            // The only return branch
            case (TokenType::RPAREN): {
                // Check if node_queue is empty
                if (node_queue.size() == 0) {
                    unexp_tok_err(*head);
                }
                // Check if first node is legal (it shouldn't be!)
                if (node_queue[0]->is_legal()) {
                    Token err_tok = node_queue[0]->get_token();
                    for (auto node: node_queue) {
                        delete node;
                    }
                    unexp_tok_err(err_tok);
                }
                // Check if number of nodes is good
                if (node_queue.size() < 3) {
                    unexp_tok_err(*head);
                }
                // Check if all other nodes are legal (they should be!)
                for (auto node = node_queue.begin() + 1;
                    node != node_queue.end(); node++) {
                    // Operators must contain legal operands
                    if (!((*node)->is_legal())) {
                        // Clear memory
                        Token err_tok = (*node)->get_token();
                        for (auto node: node_queue) {
                            delete node;
                        }
                        unexp_tok_err(err_tok);
                    }
                }
                // Create and return the root
                Operator* ret = (Operator*) node_queue[0];
                node_queue.erase(node_queue.begin());
                ret->add_oprand(node_queue);
                return ret;
            }
            case (TokenType::OPERATOR): {
                node_queue.push_back(new Operator(*(head)));
                break;
            }
            case (TokenType::NUMBER): {
                node_queue.push_back(new Number(*(head)));
                break;
            }
            default: {
                // END or ERR
                // Clear memory
                for (auto node: node_queue) {
                    delete node;
                }
                unexp_tok_err((*head));
            }
        }
    }
    // Met END before RPAREN
    for (auto node: node_queue) {
        delete node;
    }
    unexp_tok_err(*head);
    return nullptr;
}