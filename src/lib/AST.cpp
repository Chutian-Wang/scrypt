#include "AST.h"
#include "Nodes.h"
#include "Errors.h"
#include <vector>
#include <stack>
#include <string>
#include <cstring>

AST* AST::parse(const std::vector<Token> & tokens) {
    if (tokens.size() < 3) {
        if (tokens[0].type == TokenType::NUMBER) {
            return new Number(tokens[0]);
        }
        else {
            throw UnexpTokError(tokens[0]);
        }
    }
    // Parse sub-expressions
    auto head = tokens.begin();
    AST* ret = nullptr;
    try{
        ret = AST::parse(tokens, ++head);
    }
    catch (const UnexpTokError& err) {
        throw;
    }
    // Check redundent expression
    if ((++head)->type != TokenType::END) {
        delete ret;
        throw UnexpTokError(*head);
    }
    return ret;
}

// After call to parse, head is set to the last token read.
AST* AST::parse(const std::vector<Token>& tokens,
    std::vector<Token>::const_iterator& head) {

    std::vector<AST*> node_queue;

    while (1) {
        switch ((*head).type) {
            case (TokenType::LPAREN): {
                // when parse returns, head is set to one
                // token past the matching RPARAN in the
                // next cycle.
                try {
                    node_queue.push_back(
                        parse(tokens, ++head));
                }
                catch (const UnexpTokError& err) {
                    for (auto node: node_queue) {
                        delete node;
                    }
                    throw;
                }
                break;
            }
            // The only return branch
            case (TokenType::RPAREN): {
                // node_queue must have elements
                if (node_queue.size() == 0) {
                    throw UnexpTokError(*head);
                }
                if (
                    // first node cannot be legal
                    !(node_queue[0]->is_legal()) &&
                    // must have at least 1 operand
                    node_queue.size() > 1
                ) {
                    // all other nodes must be legal
                    for (auto node = node_queue.begin() + 1;
                        node < node_queue.end(); node++) {
                        if (!((*node)->is_legal())) {
                            Token err_tok = 
                                (*node)->get_token();
                            for (auto node:node_queue) {
                                delete node;
                            }
                            throw UnexpTokError(err_tok);
                        }
                    }
                    // Construct subtree
                    Operator* ret = (Operator*) node_queue[0];
                        node_queue.erase(node_queue.begin());
                        ret->add_operand(node_queue);
                        return ret;
                }
                else if (node_queue[0]->is_legal()) {
                    Token err_tok =
                        node_queue[0]->get_token();
                    for (auto node:node_queue) {
                        delete node;
                    }
                    throw UnexpTokError(err_tok);
                }
                else {
                    for (auto node:node_queue) {
                        delete node;
                    }
                    throw UnexpTokError(*head);
                }
                break;
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
                if (node_queue.size() == 1 &&
                    node_queue[0]->is_legal()) {
                    return node_queue[0];
                }
                else {
                    for (auto node: node_queue) {
                        delete node;
                    }
                    throw UnexpTokError((*head));
                }
            }
        }
        head++;
    }
    return nullptr;
}