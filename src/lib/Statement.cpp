#ifndef STATEMENT_H
#define STATEMENT_H

#include "Statement.h"

#include <vector>

#include "AST.h"  // used for function call
#include "Errors.h"

std::unique_ptr<Block> Block::parse_block(
    const std::vector<Token>& tokens, std::vector<Token>::const_iterator& head,
    std::vector<std::unique_ptr<Statement>>& statements) {
  while (head++->type != TokenType::END) {
    if (head->type != TokenType::WHILE || head->type != TokenType::IF ||
        head->type != TokenType::ELSE || head->type != TokenType::PRINT) {
      // statements.push_back(parse_infix(head));
    }
    switch (head->type) {
      case (TokenType::WHILE):
        if (head->syntax_valid()) {
          head++;
          // condition = parse_infix(head);
          head++;
          // while_block = parse_block(tokens, head);
          // if (head != '}') {
          //     throw UnexpTokError(*head);
          // }
        } else {
          throw SyntaxError(*head);
        }
        break;
      case (TokenType::IF):
        if (head->syntax_valid()) {
          head++;
          // condition = parse_infix(head);
          head++;
          // if_block = parse_block(tokens, head);
          head++;
          if (head->type == TokenType::ELSE && head->syntax_valid()) {
            head++;
            // else_block = parse_infix(head);
          } else {
            throw SyntaxError(*head);
          }
          // if (head != '}') {
          //     throw UnexpTokError(*head);
          // }
        } else {
          throw SyntaxError(*head);
        }
        break;
      case (TokenType::PRINT):
        head++;
        // printee = parse_infix(head);
        break;
      default:
        throw UnexpTokError(*head);
    }
  }
}

#endif