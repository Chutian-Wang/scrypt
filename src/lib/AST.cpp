#include "AST.h"

#include <cstring>
#include <stack>
#include <string>
#include <vector>
#include <memory>

#include "Errors.h"
#include "Nodes.h"

std::map<std::string, std::shared_ptr<AST>> symbols {};

std::shared_ptr<AST> AST::parse_S(const std::vector<Token> &tokens) {
  // Deal with short token lists
  if (tokens[0].type == TokenType::NUMBER) {
    if (tokens.size() < 3) {
      return std::shared_ptr<AST>(new Number(tokens[0]));
    } else {
      throw UnexpTokError(tokens[1]);
    }
  }
  if (tokens[0].type != TokenType::LPAREN) {
    throw UnexpTokError(tokens[0]);
  }
  // Parse sub-expressions
  auto head = tokens.begin();
  std::shared_ptr<AST> ret = nullptr;
  try {
    ret = AST::parse_S(tokens, head);
  } catch (const UnexpTokError &err) {
    throw;
  }
  // Check redundent expression
  if ((++head)->type != TokenType::END) {
    throw UnexpTokError(*head);
  }
  return ret;
}

// After call to parse_S, head is set to the last token read.
std::shared_ptr<AST> AST::parse_S(const std::vector<Token> &tokens,
                  std::vector<Token>::const_iterator &head) {
  std::vector<std::shared_ptr<AST> > node_queue;

  while ((head + 1) < tokens.end()) {
    head++;
    switch ((*head).type) {
      case (TokenType::LPAREN): {
        // when parse_S returns, head is set to one
        // token past the matching RPARAN in the
        // next cycle.
        node_queue.push_back(parse_S(tokens, head));
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
              throw UnexpTokError(err_tok);
            }
          }
          // Construct subtree
          std::shared_ptr<AST> ret = node_queue[0];
          node_queue.erase(node_queue.begin());
          ((Operator*)ret.get())->add_operand(node_queue);
          return ret;
        } else if (node_queue[0]->is_legal()) {
          Token err_tok = node_queue[0]->get_token();
          throw UnexpTokError(err_tok);
        } else {
          throw UnexpTokError(*head);
        }
        break;
      }
      case (TokenType::OPERATOR): {
        node_queue.push_back(std::shared_ptr<AST>(new Operator(*(head))));
        break;
      }
      case (TokenType::NUMBER): {
        node_queue.push_back(std::shared_ptr<AST>(new Number(*(head))));
        break;
      }
      default: {
        // Premature END or ERR
        throw UnexpTokError(*head);
      }
    }
  }
  return nullptr;
}

std::shared_ptr<AST> AST::parse_infix(const std::vector<Token> &tokens) {
  if (tokens[0].type == TokenType::END) {
    throw UnexpTokError(tokens[0]);
  }
  auto head = tokens.begin();
  auto ret = parse_infix(head);
  if ((head + 1)->type != TokenType::END) {
    // Trailing tokens
    throw UnexpTokError(*(head + 1));
  }
  return ret;
}

std::shared_ptr<AST> AST::parse_infix(std::vector<Token>::const_iterator &head) {
  std::shared_ptr<AST> lhs;
  if (head->type == TokenType::LPAREN) {
    lhs = parse_infix(++head);
    if ((head + 1)->type != TokenType::RPAREN) {
      // Unmatched RPAREN
      throw UnexpTokError(*(head + 1));
    }
    head++;
  }
  else if (head->type == TokenType::NUMBER || head->type == TokenType::IDENTIFIER) {
    lhs = parse_primary(*head);
  }
  return parse_infix(head, lhs, 0);
}

std::shared_ptr<AST> AST::parse_infix(std::vector<Token>::const_iterator &head,
                      std::shared_ptr<AST> lhs, int min_p) {
  std::shared_ptr<AST> rhs = nullptr;
  if (head->type != TokenType::NUMBER && head->type != TokenType::IDENTIFIER) {
    // Invalid first token in an expression
    if (!(head->type == TokenType::RPAREN && lhs.get() != nullptr)) throw UnexpTokError(*head);
  }
  auto peek = head + 1;
  while (peek->is_binary() && peek->get_p() >= min_p) {
    std::shared_ptr<AST> op(new Operator(*peek));
    head = peek + 1;
    if (head->type == TokenType::LPAREN) {
      rhs = parse_infix(++head);
      if ((head + 1)->type != TokenType::RPAREN) {
        // Unmatched RPAREN
        throw UnexpTokError(*(head + 1));
      }
      head++;
    }
    else {
      rhs = parse_primary(*head);
    }
    peek = head + 1;
    if (((peek)->is_binary() && (peek)->get_p() > op->get_token().get_p())
        || peek->type == TokenType::ASSIGN) {
      rhs = parse_infix(head, rhs, (peek)->get_p());
      peek = head + 1;
    }
    ((Operator *)op.get())->add_operand(lhs);
    ((Operator *)op.get())->add_operand(rhs);
    lhs = op;
  }
  return lhs;
}

std::shared_ptr<AST> AST::parse_primary(const Token& tok) {
  if (tok.type == TokenType::NUMBER) return std::shared_ptr<AST>(new Number(tok));
  else if (tok.type == TokenType::IDENTIFIER) {
    if (symbols.find(tok.text) == symbols.end()) {
      // symbol does not exist
      std::shared_ptr<AST> ret(new Identifier(tok));
      symbols[tok.text] = ret;
      return ret;
    }
    else {
      return symbols[tok.text];
    }
  }
  else {
    throw UnexpTokError(tok);
  }
}