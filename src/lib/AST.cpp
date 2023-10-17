#include "AST.h"
#include "Nodes.h"
#include "Errors.h"
#include <vector>
#include <stack>
#include <string>
#include <cstring>

AST* AST::parse(const std::vector<Token> & tokens) {
    // Parse sub-expressions
    auto head = tokens.begin();
    AST* ret;
    try{
        ret = AST::parse(tokens, ++head);
    }
    catch (const UnexpTokError err) {
        throw err;
    }
    if (head->type != TokenType::END) {
        throw UnexpTokError(*head);
    }
    return ret;
}

// After call to parse head is set to the last token read.
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
                    node_queue.push_back(parse(tokens, ++head));
                }
                catch (const UnexpTokError& err) {
                    throw err;
                }
                break;
            }
            // The only return branch
            case (TokenType::RPAREN): {
                if (
                    // first node cannot be legal
                    !node_queue[0]->is_legal() &&
                    // must have at least 1 operand
                    node_queue.size() > 1
                ) {
                    // all other nodes must be legal
                    for (auto node = node_queue.begin() + 1;
                        node < node_queue.end(); node++) {
                        if (!((*node)->is_legal())) {
                            Token err_tok = (*node)->get_token();
                            for (auto node:node_queue) {
                                delete node;
                            }
                            throw UnexpTokError(err_tok);
                        }
                        Operator* ret = node_queue[0];
                        node_queue.erase(node_queue.begin());
                        ret->add_operand(node_queue);
                        return ret;
                    }
                }
                else if (node_queue[0]->is_legal()) {
                    
                }
                else {
                    
                }
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
                if (node_queue.size() == 1 && node_queue[0]->is_legal()) {
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