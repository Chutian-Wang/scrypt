#ifndef STATEMENT_H
#define STATEMENT_H

#include <vector>
#include <memory>
#include <string>

#include <vector>

#include "Statement.h"
#include "AST.h"  // used for function call
#include "Errors.h"

std::unique_ptr<Block> Block::parse_block(
    const std::vector<Token>& tokens, std::vector<Token>::const_iterator& head,
    std::vector<std::unique_ptr<Statement>>& statements) {
  while (head++->type != TokenType::END) {
    if (head->type != TokenType::WHILE && head->type != TokenType::IF &&
        head->type != TokenType::ELSE && head->type != TokenType::PRINT) {
      // statements.push_back(parse_infix(head));
    }
    switch (head->type) {
      case (TokenType::WHILE):
        head++;
        std::unique_ptr<Expression> condition = parse_infix(head);
          
        std::unique_ptr<WhileStatement> while_statement = std::make_unique<WhileStatement>();
        while_statement->set_cond(std::move(condition));

        // std::unique_ptr<Block> while_block = Block::parse_block(tokens, head, &while_statement);
        // while_statement->set_while(std::move(while_block));

        if (head->type == TokenType::LCBRACE) {
          head++;
        }
        else {
          throw UnexpTokError(*head);
        }

        statements.push_back(std::move(while_statement));
        break;
      case (TokenType::IF):
          head++;
          std::unique_ptr<Expression> condition = parse_infix(head);

          std::unique_ptr<IfStatement> if_statement = std::make_unique<IfStatement>();
          if_statement->set_cond(std::move(condition));
         
          std::unique_ptr<Block> if_block = Block::parse_block(tokens, head, if_statement);
          if_statement->set_if(std::move(if_block));

          if (head->type == TokenType::ELSE) {
            head++;
            // else_block = parse_infix(head);
          } else {
            throw SyntaxError(*head);
          }
          // if (head != '}') {
          //     throw UnexpTokError(*head);
          // }
          else {
          throw SyntaxError(*head);
        }
        break;
      case (TokenType::PRINT):
        head++;
        std::unique_ptr<Expression> printee = parse_infix(head);

        std::unique_ptr<PrintStatement> print_statement = std::make_unique<PrintStatement>(std::move(printee));
        statements.push_back(std::move(print_statement));

        break;
      default:
        throw UnexpTokError(*head);
    }
  }
}

#endif