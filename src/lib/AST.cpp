#include "AST.h"

#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "Errors.h"
#include "Nodes.h"
#include "Token.h"
#include "Value.h"

/*************************************************************
**                    Infix Scrypt Parser                   **
*************************************************************/

/**
 * This function consumes the target Token if head == target
 * o.w. throws an unexpected token error
 */
void AST::consume(std::vector<Token>::const_iterator &head, TokenType target) {
  if (head->type == target) {
    head++;
  } else {
    throw UnexpTokError(*head);
  }
}

/**
 * This function handels function call parsing.
 */
std::vector<std::shared_ptr<AST>> AST::parse_call(
    std::vector<Token>::const_iterator &head, TokenType end) {
  // head-> arguments[0]
  std::vector<std::shared_ptr<AST>> arguments;
  while (head->type != end) {
    auto argument = AST::parse_infix(head, 0);
    arguments.push_back(std::move(argument));
    head++;
    if (head->type == TokenType::COMMA) {
      head++;
    }
  }
  // head -> )
  return arguments;
}

std::shared_ptr<AST> parse_index(std::vector<Token>::const_iterator &head) {
  head++;  // consume "["
  auto arr = AST::parse_primary(head);
  head++;
  if (head->type != TokenType::RSBRACE) throw UnexpTokError(*head);
  return arr;
}

std::shared_ptr<AST> parse_arr(std::vector<Token>::const_iterator &head) {
  head++;  // consume "["
  auto arr = std::make_shared<Array>();
  while (head->type != TokenType::RSBRACE && head->type != TokenType::END) {
    arr->add_literal(AST::parse_infix(head, 0));
    head++;
    if (head->type == TokenType::COMMA) head++;
  }
  if (head->type != TokenType::RSBRACE) throw UnexpTokError(*head);
  return arr;
}

std::shared_ptr<AST> AST::parse_infix(std::vector<Token>::const_iterator &head,
                                      int min_p) {
  std::shared_ptr<AST> lhs = parse_primary(head);

  // Check precedence
  while (p_map.count((head + 1)->text) && ((head + 1)->get_p() > min_p)) {
    head++;
    int precedence = head->get_p();
    if (head->text == "=") {
      // handles right-associative
      precedence -= 1;
    }
    if (head->text == "(") {
      // parse function call
      auto args = parse_call(++head, TokenType::RPAREN);
      std::shared_ptr<AST> new_lhs(new FunctionCall(lhs, args));
      lhs = new_lhs;
      continue;
    }
    if (head->text == "[") {
      // parse arr index
      auto index = parse_index(head);
      std::shared_ptr<AST> new_lhs = std::make_shared<Array>();
      ((Array *)new_lhs.get())->set_identifier(lhs);
      ((Array *)new_lhs.get())->set_acc_index(index);
      lhs = new_lhs;
      continue;
    }
    std::shared_ptr<AST> op(new Operator(*head));
    std::shared_ptr<AST> rhs = parse_infix(++head, precedence);

    ((Operator *)op.get())->add_operand(lhs);
    ((Operator *)op.get())->add_operand(rhs);
    lhs = op;
  }

  return lhs;
}

std::shared_ptr<AST> AST::parse_primary(
    std::vector<Token>::const_iterator &head) {
  if (head->type == TokenType::NUMBER || head->type == TokenType::BOOL ||
      head->type == TokenType::null) {
    return std::shared_ptr<AST>(new Constant(*head));
  } else if (head->type == TokenType::IDENTIFIER) {
    return std::shared_ptr<AST>(new Identifier(*head));
  } else if (head->type == TokenType::LPAREN) {
    head++;  // consume "(""
    std::shared_ptr<AST> node = parse_infix(head, 0);
    head++;  // put head at ")"
    return node;
  } else if (head->type == TokenType::LSBRACE) {
    // parse array literal
    std::shared_ptr<AST> arr = parse_arr(head);
    return arr;
  } else {
    throw UnexpTokError(*head);
  }
}

// std::shared_ptr<AST> AST::parse_infix(const std::vector<Token> &tokens) {
//   if (tokens[0].type == TokenType::END) {
//     throw UnexpTokError(tokens[0]);
//   }
//   auto head = tokens.begin();
//   auto ret = parse_infix(head);
//   if ((head + 1)->type != TokenType::END) {
//     // Trailing tokens
//     throw UnexpTokError(*(head + 1));
//   }
//   return ret;
// }

// std::shared_ptr<AST> AST::parse_infix(
//     std::vector<Token>::const_iterator &head) {
//   std::shared_ptr<AST> lhs;
//   if (head->type == TokenType::LPAREN) {
//     lhs = parse_infix(++head);
//     if ((head + 1)->type != TokenType::RPAREN) {
//       // Unmatched RPAREN
//       throw UnexpTokError(*(head + 1));
//     }
//     head++;
//   } else if (head->type == TokenType::NUMBER ||
//              head->type == TokenType::IDENTIFIER ||
//              head->type == TokenType::BOOL) {
//     // Invalid first token will get handled by parse_primary
//     lhs = parse_primary(*head);
//   }
//   return parse_infix(head, lhs, 0);
// }

// std::shared_ptr<AST> AST::parse_infix(std::vector<Token>::const_iterator
// &head,
//                                       std::shared_ptr<AST> lhs, int min_p) {
//   std::shared_ptr<AST> rhs = nullptr;
//   if (head->type != TokenType::NUMBER && head->type != TokenType::IDENTIFIER
//   &&
//       head->type != TokenType::BOOL) {
//     // Invalid first token in an expression
//     if (!(head->type == TokenType::RPAREN && lhs.get() != nullptr)){
//       throw UnexpTokError(*head);
//     }
//   }
//   auto peek = head + 1;
//   // Same level loop
//   while (peek->is_binary() && peek->get_p() >= min_p) {
//     std::shared_ptr<AST> op(new Operator(*peek));
//     head = peek + 1;
//     if (head->type == TokenType::LPAREN) {
//       rhs = parse_infix(++head);
//       if ((head + 1)->type != TokenType::RPAREN) {
//         // Unmatched RPAREN
//         throw UnexpTokError(*(head + 1));
//       }
//       head++;
//     } else {
//       rhs = parse_primary(*head);
//     }
//     peek = head + 1;
//     // Higher level loop
//     while (((peek)->is_binary() && (peek)->get_p() > op->get_token().get_p())
//     /*||
//            peek->text == "="*/) {

//       int precedence = (peek)->get_p();
//       if((peek)->text == "=") {
//           // It's right associative
//           precedence = 1;
//       }
//       rhs = parse_infix(head, rhs, precedence);
//       peek = head + 1;
//     }
//     // if (op->get_token().text == "=") {
//     //   if (lhs->get_token().type != TokenType::IDENTIFIER) {
//     //     // Only identifier is allowed on lhs for assignment
//     //     throw UnexpTokError(op->get_token());
//     //   }
//     // }
//     ((Operator *)op.get())->add_operand(lhs);
//     ((Operator *)op.get())->add_operand(rhs);
//     lhs = op;
//   }
//   return lhs;
// }

// std::shared_ptr<AST> AST::parse_primary(const Token &tok) {
//   if (tok.type == TokenType::NUMBER || tok.type == TokenType::BOOL ||
//   tok.type == TokenType::null) {
//     return std::shared_ptr<AST>(new Constant(tok));
//   } else if (tok.type == TokenType::IDENTIFIER) {
//     return std::shared_ptr<AST>(new Identifier(tok));
//   } else {
//     throw UnexpTokError(tok);
//   }
// }

/************************** S-parser *************************
**                         DEPRECIATED                      **
**************************************************************

std::vector<std::shared_ptr<AST>> AST::parse_S_multiple(
    const std::vector<Token> &tokens) {
  std::vector<std::shared_ptr<AST>> expressions;
  auto head = tokens.begin();
  std::shared_ptr<AST> ret = nullptr;
  while ((head)->type != TokenType::END) {
    std::shared_ptr<AST> ret = AST::parse_S_top(head);
    expressions.push_back(ret);
    head++;
  }
  return expressions;
}

std::shared_ptr<AST> AST::parse_S_top(
    std::vector<Token>::const_iterator &head) {
  // Deal with short token lists
  if (((head + 1)->type == TokenType::OPERATOR) &&
      ((head + 2)->type == TokenType::RPAREN)) {
    throw UnexpTokError(*(head + 2));
  }
  if ((*head).type == TokenType::NUMBER) {
    if (((head + 1)->type == TokenType::LPAREN) ||
        ((head + 1)->type == TokenType::END)) {
      return std::shared_ptr<AST>(new Number(*(head)));
    } else {
      throw UnexpTokError(*(head + 1));
    }
  }
  if ((*head).type != TokenType::LPAREN) {
    throw UnexpTokError(*head);
  }
  // Parse sub-expressions
  std::shared_ptr<AST> ret;
  ret = AST::parse_S(head);
  return ret;
}

// After call to parse_S, head is set to the last token read.
std::shared_ptr<AST> AST::parse_S(std::vector<Token>::const_iterator &head) {
  std::vector<std::shared_ptr<AST>> node_queue;
  auto head0 = head;
  while (1) {
    head++;
    switch ((*head).type) {
      case (TokenType::LPAREN): {
        // when parse_S returns, head is set to one
        // token past the matching RPARAN in the
        // next cycle.
        head0 = head;
        node_queue.push_back(parse_S(head));
        break;
      }
      // This is the only return branch
      case (TokenType::RPAREN): {
        // node_queue must have elements
        if (node_queue.size() == 0) {
          throw UnexpTokError(*head);
        }
        // Case 1: node_queue[0] is not '='
        if ((!(node_queue[0]->is_legal()) &&  // First node cannot be legal
             node_queue.size() > 1) &&        // Must have at least 1 operand
            node_queue[0]->get_token().text[0] !=
                '='  // Deal with assignment separately
        ) {
          if (node_queue.size() == 2) {
            // Operator cannot perform any functions on just identifier
            // e.g. (+2) --> is okay
            // e.g. (+a) --> not okay
            if ((node_queue[1]->get_token().type == TokenType::IDENTIFIER)) {
              throw UnexpTokError(*head0);
            }
          }
          // Deal with regular operators
          // All operand nodes must be legal
          for (auto node = node_queue.begin() + 1; node < node_queue.end();
               node++) {
            // Only identifiers can be illegal
            if (!((*node)->is_legal()) &&
                !((*node)->get_token().type == TokenType::IDENTIFIER)) {
              Token err_tok = (*node)->get_token();
              throw UnexpTokError(err_tok);
            }
          }
          // Construct subtree
          std::shared_ptr<AST> ret = node_queue[0];
          node_queue.erase(node_queue.begin());
          ((Operator *)ret.get())
              ->add_operand(node_queue);  // SEG FAULTING HERE?
          return ret;
        }  // Case 2: node_queue[0] is  '='
        else if (node_queue[0]->get_token().text[0] == '=') {
          // Deal with assignment
          // Assignment cannot already be legal
          if (node_queue[0]->is_legal()) {
            throw UnexpTokError(node_queue[0]->get_token());
          }
          for (auto node = node_queue.begin() + 1; node < node_queue.end();
               node++) {
            if (node != node_queue.end() - 1 &&
                (*node)->get_token().type != TokenType::IDENTIFIER) {
              if ((*(node + 1))->get_token().type == TokenType::IDENTIFIER) {
                throw UnexpTokError((*(node + 1))->get_token());
              }
              throw UnexpTokError((*(node + 1))->get_token());
            }
          }
          // Ensures right most node is legal
          if (!node_queue.back()->is_legal()) {
            throw UnexpTokError(*head);
          }
          // Construct subtree
          std::shared_ptr<AST> ret = node_queue[0];
          node_queue.erase(node_queue.begin());
          ((Operator *)ret.get())->add_operand(node_queue);
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
        if (((head)->text[0] == '=')) {
          if (((head + 1)->type != TokenType::IDENTIFIER) &&
              ((head + 1)->type != TokenType::LPAREN)) {
            throw UnexpTokError(*(head + 1));
          }
        }
        node_queue.push_back(std::shared_ptr<AST>(new Operator(*(head))));
        break;
      }
      case (TokenType::NUMBER): {
        node_queue.push_back(std::shared_ptr<AST>(new Number(*(head))));
        break;
      }
      case (TokenType::IDENTIFIER): {
        node_queue.push_back(std::shared_ptr<AST>(new Identifier(*(head))));
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
**************************************************************/