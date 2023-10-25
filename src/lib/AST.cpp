#include "AST.h"

#include <cstring>
#include <stack>
#include <string>
#include <vector>
#include <map>

#include "Errors.h"
#include "Nodes.h"

std::map<std::string, Identifier*> symbols {};

std::vector<AST*> AST::parse_S_multiple(const std::vector<Token> &tokens) {
  std::vector<AST*> multiple_S = {};
  auto head = tokens.begin();
  AST *ret = nullptr;
  while ((head)->type != TokenType::END) {
    try {
      ret = AST::parse_S_short(head);
      multiple_S.push_back(ret);
    } catch (const UnexpTokError &err) {
      delete ret;
      throw;
    }
    head++;
  }
  return multiple_S;
}

AST *AST::parse_S_short(std::vector<Token>::const_iterator &head) {
  // Deal with short token lists
  if ((*head).type == TokenType::NUMBER) {
    if (((head + 1)->type == TokenType::LPAREN) || ((head + 1)->type == TokenType::END)) {
      return new Number(*(head));
    } else {
      throw UnexpTokError(*(head + 1));
    }
  }
  if ((*head).type != TokenType::LPAREN) {
    throw UnexpTokError(*head);
  }
  // Parse sub-expressions
  AST *ret = nullptr;
  try {
    ret = AST::parse_S(head);
  } catch (const UnexpTokError &err) {
    throw;
  }
  return ret;
}

// After call to parse_S, head is set to the last token read.
AST *AST::parse_S(std::vector<Token>::const_iterator &head) {
  std::vector<AST *> node_queue;

  while (1) {
    head++;
    switch ((*head).type) {
      case (TokenType::LPAREN): {
        // when parse_S returns, head is set to one
        // token past the matching RPARAN in the
        // next cycle.
        try {
          node_queue.push_back(parse_S(head));
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
            // Normal operators
            // First node cannot be legal
            (!(node_queue[0]->is_legal()) &&
            // Must have at least 1 operand
            node_queue.size() > 1) ||
            // Assignment needs to be legal
            (node_queue[0]->get_token().type == TokenType::IDENTIFIER &&
            node_queue[0]->is_legal())
            ) {
          // All other nodes must be legal
          for (auto node = node_queue.begin() + 1; node < node_queue.end();
               node++) {
            if (!((*node)->is_legal()) && !((*node)->get_token().type == TokenType::IDENTIFIER)) {
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
      case (TokenType::IDENTIFIER): {
        node_queue.push_back(new Identifier(*(head)));
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