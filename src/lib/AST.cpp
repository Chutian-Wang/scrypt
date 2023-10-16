#include "AST.h"
#include "Nodes.h"
#include "Errors.h"
#include <vector>
#include <stack>
#include <string>
#include <cstring>

AST* AST::parse(const std::vector<Token> & tokens) {
    // Check if given empty expression
    if (tokens.size() == 1 &&
        tokens[0].type == TokenType::END) {
        throw UnexpTokError(tokens[0]);
    };
    // Check if a single number is recieved
    if (tokens[0].type == TokenType::NUMBER) {
        if (tokens.size() == 2) {
            return new Number(tokens[0]);
        }
        else {
            throw UnexpTokError(tokens[1]);
        }
    }
    // Check if fist token is LPRAREN
    if (tokens[0].type != TokenType::LPAREN) {
        throw UnexpTokError(tokens[0]);
    }
    // Check if second token is number
    if (tokens[1].type == TokenType::NUMBER) {
        throw UnexpTokError(tokens[1]);
    }
    // Parse sub-expressions
    auto head = tokens.begin();
    AST* ret;
    try {
        ret = AST::parse(tokens, head);
    }
    catch (const std::exception& err) {
        throw err;
    }
    
    head++;
    // Check if something is left after a complete
    // expression is parsed
    if ((*head).type != TokenType::END) { 
        delete ret;
        throw UnexpTokError(*(head));
    }
    return ret;
}

// After call to parse head is set to the last token read.
AST* AST::parse(const std::vector<Token>& tokens,
    std::vector<Token>::const_iterator& head) {

    std::vector<AST*> node_queue;

    while (head < tokens.end() &&
        (*head).type != TokenType::END) {
        head++;
        switch ((*head).type) {
            case (TokenType::LPAREN): {
                // when parse returns, head is set to one
                // token past the matching RPARAN in the
                // next cycle.
                try {
                    node_queue.push_back(parse(tokens, head));
                }
                catch (const std::exception& err) {
                    for (auto node: node_queue) {
                        delete node;
                    }
                    throw err;
                }
                break;
            }
            // The only return branch
            case (TokenType::RPAREN): {
                // Check if first node is legal
                // (it shouldn't be!)
                if (node_queue[0]->is_legal()) {
                    Token err_tok = node_queue[0]->get_token();
                    for (auto node: node_queue) {
                        delete node;
                    }
                    throw UnexpTokError(err_tok);
                }
                // Check if number of nodes is good
                if (node_queue.size() < 2) {
                    for (auto node: node_queue){
                        delete node;
                    }
                    throw UnexpTokError(*head);
                }
                // Check if all other nodes are legal
                // (they should be!)
                for (auto node = node_queue.begin() + 1;
                    node != node_queue.end(); node++) {
                    // Operators must contain legal operands
                    if (!((*node)->is_legal())) {
                        // Clear memory
                        Token err_tok = (*node)->get_token();
                        for (auto node: node_queue) {
                            delete node;
                        }
                        throw UnexpTokError(err_tok);
                    }
                }
                // Create and return the root
                Operator* ret = (Operator*) node_queue[0];
                node_queue.erase(node_queue.begin());
                ret->add_operand(node_queue);
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
                throw UnexpTokError((*head));
            }
        }
    }
    // Met END before RPAREN
    for (auto node: node_queue) {
        delete node;
    }
    throw UnexpTokError(*head);
    return nullptr;
}