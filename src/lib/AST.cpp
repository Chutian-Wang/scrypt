#include "AST.h"

#include <cstring>
#include <stack>
#include <string>
#include <vector>
#include <map>
#include <iostream>
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

// Infix parser
AST *AST::parse_infix(const std::vector<Token> &tokens) {
    size_t start = 0; // Initialize the starting position.
    AST *lhs = parse_primary(tokens, start);
    return parse_infix(lhs, 0, tokens, start);
}

AST *AST::parse_infix(AST *lhs, int minP, const std::vector<Token> &tokens, size_t &start) {
  while (start < tokens.size()) {
    const Token &lookahead = tokens[start];
    if (lookahead.type == TokenType::OPERATOR || lookahead.type == TokenType::ASSIGN) {
        char op = lookahead.text[0];

      if (Token::Precedence.count(op) > 0 && Token::Precedence.at(op) >= minP) {
        ++start;

      AST *rhs = parse_primary(tokens, start);

        while (start < tokens.size() && (tokens[start].type == TokenType::OPERATOR || tokens[start].type == TokenType::ASSIGN)) {
          char nextOp = tokens[start].text[0];
          int nextP = Token::Precedence.at(nextOp);

          if (nextP > Token::Precedence.at(op) || (nextP == Token::Precedence.at(op) && nextOp == '=')) {
            rhs = parse_infix(rhs, nextP + 1, tokens, start);
          } 
          else {
            break;
          }
        }

      if (op == '+') {
        lhs += rhs;
      } 
      else if (op == '-') {
        lhs -= rhs;
      } 
      else if (op == '*') {
        lhs *= rhs;
      } 
      else {
        lhs /= rhs;
      }
    } 
    else {
      break;
    }
  } 
  else {
    break;
  }
  }
return lhs;
}

AST *AST::parse_primary(const std::vector<Token> &tokens, size_t &start) {
    if (start >= tokens.size()) {
        return nullptr;
    }

    const Token &token = tokens[start];

    if (token.type == TokenType::NUMBER) {
        double value = std::stod(token.text);
        ++start;
        return new NumericAST(value);
    } else if (token.type == TokenType::IDENTIFIER) {
        std::string variableName = token.text;
        ++start;
        return new VariableAST(variableName);
    } else if (token.type == TokenType::LPAREN) {
        ++start;
        AST *expression = parse_infix(nullptr, 0, tokens, start);

        if (start < tokens.size() && tokens[start].type == TokenType::RPAREN) {
            ++start;
        } else {
            // Handle missing closing parenthesis as an error.
            throw SyntaxError(start);
        }
        return expression;
    }

    // Handle other cases for functions, constants, or additional primary constructs.
    // You should expand this function based on your specific language's primary expressions.

    return nullptr;
}



//   lookahead++;

//   int parenCount;
//   while ((*lookahead).type == TokenType::LPAREN) {
//     lookahead++;
//     ++parenCount;
//   }

//   if ((*lookahead).type != TokenType::LPAREN) {
//       // int begin = (tokens.size() - lookahead.size()) - 1;
//       // think of a way to get lookahead.size()
//       // arse_infix(tokens[begin]);
//   }
  
//   char op = lookahead;
//   while (((*lookahead).type == TokenType::OPERATOR || (*lookahead).type == TokenType::ASSIGN) && 
//           Token::Precedence.at(op) >= minP) {
//     AST *rhs = lookahead++;
//     while(((*lookahead).type == TokenType::OPERATOR || (*lookahead).type == TokenType::ASSIGN)  && 
//           (Token::Precedence.at(op) > minP || op == '=')) {
//       if (Token::Precedence.at(lookahead) > Token::Precedence.at(op)) {
//         rhs = parse_infix(rhs, Token::Precedence.at(op) + 1, tokens);
//       }
//       else {
//         rhs = parse_infix(rhs, Precedence.at(op), tokens);
//       }
//       lookahead++;
//     }

//     //solve for lhs + op + rhs and assign it to lhs
//     if (op == '+') {
//       lhs = lhs + rhs;
//     }
//     else if (op == '-') {
//       lhs = lhs - rhs;
//     }
//     else if (op == '*') {
//       lhs = lhs * rhs;
//     }
//     else {
//       lhs = lhs / rhs;
//     }

//   }
//   return lhs;
// }
