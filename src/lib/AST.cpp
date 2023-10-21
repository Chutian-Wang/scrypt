#include "AST.h"

#include <cstring>
#include <stack>
#include <string>
#include <vector>

#include "Errors.h"
#include "Nodes.h"

AST *AST::parse_S(const std::vector<Token> &tokens) {
  // Deal with short token lists
  if (tokens[0].type == TokenType::NUMBER) {
    if (tokens.size() < 3) {
      return new Number(tokens[0]);
    } else {
      throw UnexpTokError(tokens[1]);
    }
  }
  if (tokens[0].type != TokenType::LPAREN) {
    throw UnexpTokError(tokens[0]);
  }
  // Parse sub-expressions
  auto head = tokens.begin();
  AST *ret = nullptr;
  try {
    ret = AST::parse_S(tokens, head);
  } catch (const UnexpTokError &err) {
    throw;
  }
  // Check redundent expression
  if ((++head)->type != TokenType::END) {
    delete ret;
    throw UnexpTokError(*head);
  }
  return ret;
}

// After call to parse_S, head is set to the last token read.
AST *AST::parse_S(const std::vector<Token> &tokens,
                  std::vector<Token>::const_iterator &head) {
  std::vector<AST *> node_queue;

  while ((head + 1) < tokens.end()) {
    head++;
    switch ((*head).type) {
      case (TokenType::LPAREN): {
        // when parse_S returns, head is set to one
        // token past the matching RPARAN in the
        // next cycle.
        try {
          node_queue.push_back(parse_S(tokens, head));
        } catch (const UnexpTokError &err) {
          for (auto node : node_queue) {
            delete node;
          }
          throw;
        }
        break;
      }
      // This is the only return branch
      case (TokenType::RPAREN): {
        // node_queue must have elements
        if (node_queue.size() == 0) {
          throw UnexpTokError(*head);
        }
        if (
            // First node cannot be legal
            !(node_queue[0]->is_legal()) &&
            // Must have at least 1 operand
            node_queue.size() > 1) {
          // All other nodes must be legal
          for (auto node = node_queue.begin() + 1; node < node_queue.end();
               node++) {
            if (!((*node)->is_legal())) {
              Token err_tok = (*node)->get_token();
              for (auto node : node_queue) {
                delete node;
              }
              throw UnexpTokError(err_tok);
            }
          }
          // Construct subtree
          Operator *ret = (Operator *)node_queue[0];
          node_queue.erase(node_queue.begin());
          ret->add_operand(node_queue);
          return ret;
        } else if (node_queue[0]->is_legal()) {
          Token err_tok = node_queue[0]->get_token();
          for (auto node : node_queue) {
            delete node;
          }
          throw UnexpTokError(err_tok);
        } else {
          for (auto node : node_queue) {
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
        // Premature END or ERR
        // Clear memory
        for (auto node : node_queue) {
          delete node;
        }
        throw UnexpTokError(*head);
      }
    }
  }
  return nullptr;
}