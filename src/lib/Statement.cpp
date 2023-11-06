#ifndef STATEMENT_H
#define STATEMENT_H

#include "Statement.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "AST.h"
#include "Errors.h"
#include "Nodes.h"
#include "Value.h"

Block::Block() { this->statements = std::vector<std::unique_ptr<Statement>>(); }

Block::~Block() {
  // Auto garbage collection
}

// This function will set head to one past the last token
// read unless the last token is END, in which case it will
// be set to END.
std::unique_ptr<Block> Block::parse_block(
    const std::vector<Token>& tokens,
    std::vector<Token>::const_iterator& head) {
  auto block = std::make_unique<Block>();

  while ((head + 1)->type != TokenType::END) {
  while (head->type != TokenType::END && head->type != TokenType::RCBRACE) {
    if (head->type != TokenType::WHILE && head->type != TokenType::IF &&
        head->type != TokenType::ELSE && head->type != TokenType::PRINT) {
      block->add_statement(
          std::make_unique<Expression>(AST::parse_infix(head)));
      head++;
    } else {
      switch (head->type) {
        case (TokenType::WHILE): {
          head++;
          auto condition = std::make_unique<Expression>(AST::parse_infix(head));
          head++;

          if (head->type != TokenType::LCBRACE) {
            head++;
          }
          else {
            throw UnexpTokError(*head);
          }

          std::unique_ptr<WhileStatement> while_statement =
              std::make_unique<WhileStatement>();
          while_statement->set_cond(condition);

          std::unique_ptr<Block> while_block = Block::parse_block(tokens, head);
          while_statement->set_while(while_block);

          if (head->type == TokenType::RCBRACE) {
            head++;
          } else {
            throw UnexpTokError(*head);
          }

          block->add_statement(std::move(while_statement));
        } break;
        case (TokenType::IF): {
          head++;
          auto condition =
              std::make_unique<Expression>(Expression(AST::parse_infix(head)));
          head++;
          if (head->type == TokenType::LCBRACE) {
            head++;
          } else {
            throw UnexpTokError(*head);
          }

          std::unique_ptr<IfStatement> if_statement =
              std::make_unique<IfStatement>();
          if_statement->set_cond(condition);

          std::unique_ptr<Block> if_block = Block::parse_block(tokens, head);
          if_statement->set_if(if_block);

          if (head->type == TokenType::RCBRACE) {
            head++;
          } else {
            throw UnexpTokError(*head);
          }

          if (head->type == TokenType::ELSE) {
            head++;
            std::unique_ptr<Block> else_block =
                Block::parse_block(tokens, head);
            if_statement->set_else(else_block);
          }
          block->add_statement(std::move(if_statement));
        } break;
        case (TokenType::PRINT): {
          head++;
          auto printee =
              std::make_unique<Expression>(Expression(AST::parse_infix(head)));
          head++;
          auto print_statement =
              std::make_unique<PrintStatement>(printee, std::cout);
          block->add_statement(std::move(print_statement));
        } break;
        default:
          throw UnexpTokError(*head);
      }
    }
  }
  }
  if (head->type == TokenType::RCBRACE) head++;
  return block;
}

void Block::add_statement(std::unique_ptr<Statement> statement) {
  // The unique pointer must be moved not copied
  this->statements.push_back(std::move(statement));
}

void Block::run() {
  // Use const reference for unique pointer here
  for (auto const& statement : this->statements) {
    statement->run();
  }
}

void Block::print(std::ostream& os, int depth) const {
  for (auto const& statement : this->statements) {
    for (int i = 0; i < depth; i++) {
      os << "    ";
    }
    statement->print(os, depth);
  }
}

Expression::Expression(std::shared_ptr<AST> expr) { this->expr = expr; }

Expression::~Expression() {
  // Auto garbage collection
}

void Expression::run() { this->expr->eval(); }

void Expression::print(std::ostream& os, int depth) const {
  for (int i = 0; i < depth; i++) {
    os << "    ";
  }
  this->expr->get_infix(os);
  os << '\n';
}

Value Expression::eval() { return this->expr->eval(); }

void Expression::get_infix(std::ostream& os) { this->expr->get_infix(os); }

IfStatement::IfStatement() {
  this->condition = std::unique_ptr<Expression>();
  this->if_block = std::unique_ptr<Block>();
  this->else_block = std::unique_ptr<Block>();
}

IfStatement::~IfStatement() {
  // Auto garbage collection
}

void IfStatement::run() {
  Value truth = condition->eval();
  if (truth.type == ValueType::BOOL) {
    if (truth._value._bool) {
      this->if_block->run();
    } else {
      // Do else block only if it exists
      if (this->else_block.get() != nullptr) {
        this->else_block->run();
      }
    }
  } else {
    throw InvalidCond();
  }
}

void IfStatement::print(std::ostream& os, int depth) const {
  for (int i = 0; i < depth; i++) {
    os << "    ";
  }
  os << "if ";
  this->condition->get_infix(os);
  os << " {\n";
  this->if_block->print(os, depth + 1);
  for (int i = 0; i < depth; i++) {
    os << "    ";
  }
  os << "}";
  if (this->else_block.get() != nullptr) {
    os << " else {\n";
    this->else_block->print(os, depth + 1);
    for (int i = 0; i < depth; i++) {
      os << "    ";
    }
    os << "}";
  }
  os << '\n';
}

void IfStatement::set_cond(std::unique_ptr<Expression>& cond) {
  this->condition = std::move(cond);
}

void IfStatement::set_if(std::unique_ptr<Block>& block) {
  this->if_block = std::move(block);
}

void IfStatement::set_else(std::unique_ptr<Block>& block) {
  this->else_block = std::move(block);
}

WhileStatement::WhileStatement() {
  this->condition = std::unique_ptr<Expression>();
  this->while_block = std::unique_ptr<Block>();
}

WhileStatement::~WhileStatement() {
  // Auto garbage collection
}

void WhileStatement::run() {
  while (1) {
    Value truth = condition->eval();
    if (truth.type == ValueType::BOOL) {
      if (truth._value._bool) {
        this->while_block->run();
      } else {
        break;
      }
    } else {
      throw InvalidCond();
    }
  }
}

void WhileStatement::print(std::ostream& os, int depth) const {
  for (int i = 0; i < depth; i++) {
    os << "    ";
  }
  os << "while ";
  this->condition->get_infix(os);
  os << " {\n";
  this->while_block->print(os, depth + 1);
  for (int i = 0; i < depth; i++) {
    os << "    ";
  }
  os << "}\n";
}

void WhileStatement::set_cond(std::unique_ptr<Expression>& cond) {
  this->condition = std::move(cond);
}

void WhileStatement::set_while(std::unique_ptr<Block>& block) {
  this->while_block = std::move(block);
}

PrintStatement::PrintStatement(std::unique_ptr<Expression>& printee,
                               std::ostream& os)
    : os(os) {
  this->printee = std::move(printee);
}

PrintStatement::~PrintStatement() {
  // Auto garbage collection
}

void PrintStatement::run() { this->os << this->printee->eval() << '\n'; }

void PrintStatement::print(std::ostream& os, int depth) const {
  for (int i = 0; i < depth; i++) {
    os << "    ";
  }
  os << "print ";
  this->printee->get_infix(os);
  os << '\n';
}

#endif
