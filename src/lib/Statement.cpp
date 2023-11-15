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

std::unique_ptr<Block> Block::parse_block(const std::vector<Token>& tokens) {
  auto head = tokens.begin();
  auto block = Block::parse_block(head);
  if (head->type != TokenType::END) {
    throw UnexpTokError(*head);
  }
  return block;
}

std::unique_ptr<Block> Block::parse_block(
    std::vector<Token>::const_iterator& head) {
  auto block = std::make_unique<Block>();

  while ((head + 1)->type != TokenType::END) {
  while (head->type != TokenType::END && head->type != TokenType::RCBRACE) {
    if (head->type == TokenType::LCBRACE) {
      block = Block::parse_block(++head);
      if (head->type != TokenType::RCBRACE) {
        throw UnexpTokError(*head);
      }
      head++;
      return block;
    } else if (head->type != TokenType::WHILE && head->type != TokenType::IF &&
               head->type != TokenType::ELSE && head->type != TokenType::PRINT &&
               head->type != TokenType::FUNCTION && head->type != TokenType::RETURN) {
      block->add_statement(
          std::make_unique<Expression>(AST::parse_infix(head)));
          if ((head+1)->type == TokenType::SEMICOLON){
              // ignore semicolon
              head++;
          } else{
                  throw UnexpTokError(*(head+1));
              }
      head++;
    } else {
      switch (head->type) {
        case (TokenType::WHILE): {
          head++;
          auto condition = std::make_unique<Expression>(AST::parse_infix(head));
          head++;
          if (head->type != TokenType::LCBRACE) {
            throw UnexpTokError(*head);
          }

          std::unique_ptr<WhileStatement> while_statement =
              std::make_unique<WhileStatement>();
          while_statement->set_cond(condition);

          std::unique_ptr<Block> while_block = Block::parse_block(head);
          while_statement->set_while(while_block);

          block->add_statement(std::move(while_statement));
        } break;
        case (TokenType::IF): {
          head++;
          auto condition =
              std::make_unique<Expression>(Expression(AST::parse_infix(head)));
          head++;
          if (head->type != TokenType::LCBRACE) {
            throw UnexpTokError(*head);
          }

          std::unique_ptr<IfStatement> if_statement =
              std::make_unique<IfStatement>();
          if_statement->set_cond(condition);

          std::unique_ptr<Block> if_block = Block::parse_block(head);
          if_statement->set_if(if_block);

          if (head->type == TokenType::RCBRACE) {
            head++;
          } else {
            throw UnexpTokError(*head);
          }

          if (head->type == TokenType::ELSE) {
            head++;
            std::unique_ptr<Block> else_block = Block::parse_block(head);
            if_statement->set_else(else_block);
          }
          block->add_statement(std::move(if_statement));
        } break;
        case (TokenType::PRINT): {
          head++;
          auto printee =
              std::make_unique<Expression>(Expression(AST::parse_infix(head)));
              if ((head+1)->type == TokenType::SEMICOLON){
              // ignore semicolon
              head++;
              } else{
                  throw UnexpTokError(*(head+1));
              }
          head++;
          auto print_statement =
              std::make_unique<PrintStatement>(printee, std::cout);
          block->add_statement(std::move(print_statement));
        } break;
        case (TokenType::FUNCTION): {
            head++;
            auto name = AST::parse_primary(*head);
            //head->foo
            head++;
            std::unique_ptr<FunctStatement> funct_statement =
              std::make_unique<FunctStatement>();
            funct_statement->set_name(name);
            if (head->type == TokenType::LPAREN){
                head++;
                if(head->type != TokenType::RPAREN){
                    do{
                        if(head->type == TokenType::COMMA){head++;}
                        auto argument = AST::parse_primary(*head);
                        funct_statement->add_argument(argument);
                        head++;
                        if(head->type == TokenType::RPAREN){break;}
                    } while(head->type == TokenType::COMMA);
                } 
            }
            head++;
            std::unique_ptr<Block> funct_block = Block::parse_block(head);
            funct_statement->set_function(funct_block);
            block->add_statement(std::move(funct_statement));
        } break;
        case (TokenType::RETURN): {
            head++;
            std::unique_ptr<ReturnStatement> return_statement =
              std::make_unique<ReturnStatement>();
            auto ret = std::make_unique<Expression>(AST::parse_infix(head));
            return_statement->set_return(ret);
            head++;
            if (head->type == TokenType::SEMICOLON){
              // ignore semicolon
              head++;
            } else{
                  throw UnexpTokError(*(head+1));
              }
            block->add_statement(std::move(return_statement));
        } break;
        default:
          throw UnexpTokError(*head);
      }
    }
  }
  }
  // if (head->type == TokenType::RCBRACE) head++;
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
  os << ';' << '\n';
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
    if (std::get<bool>(truth._value)) {
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
  os << "}\n";
  if (this->else_block.get() != nullptr) {
    for (int i = 0; i < depth; i++) {
      os << "    ";
    }
    os << "else {\n";
    this->else_block->print(os, depth + 1);
    for (int i = 0; i < depth; i++) {
      os << "    ";
    }
    os << "}\n";
  }
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
      if (std::get<bool>(truth._value)) {
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
  os << ';' << '\n';
}

FunctStatement::FunctStatement() {
  this->name = std::shared_ptr<AST>();
  this->function_block = std::unique_ptr<Block>();
}

FunctStatement::~FunctStatement() {
  // Auto garbage collection
}

void FunctStatement::add_argument(std::shared_ptr<AST>& arg){
    this->arguments.push_back(std::move(arg));
}

void FunctStatement::set_name(std::shared_ptr<AST>& def){
    this->name = std::move(def);
}

void FunctStatement::set_function(std::unique_ptr<Block>& block){
    this->function_block = std::move(block);
}

void FunctStatement::run() {
  return;
}

void FunctStatement::print(std::ostream& os, int depth) const {
  for (int i = 0; i < depth; i++) {
    os << "    ";
  }
  os << "def ";
  this->name->get_infix(os);
  os << '(';
  this->arguments.at(0)->get_infix(os);
  bool first = true;
  for (auto const& arg : this->arguments){
      if(first){
          first = false;
          continue;}
      os << ", ";
      arg->get_infix(os);
  }
  os << ") {\n";
  this->function_block->print(os, depth + 1);
  for (int i = 0; i < depth; i++) {
    os << "    ";
  }
  os << "}\n";
}

ReturnStatement::ReturnStatement() {
  return;
}

ReturnStatement::~ReturnStatement() {
  // Auto garbage collection
}

void ReturnStatement::run(){
    return;
}

void ReturnStatement::print(std::ostream& os, int depth) const{
    for (int i = 0; i < depth; i++) {
        os << "    ";
    }
    os << "return ";
    this->ret->get_infix(os);
    os << ';' << '\n';
}

void ReturnStatement::set_return(std::unique_ptr<Expression>& value){
    this->ret = std::move(value);
}

#endif
