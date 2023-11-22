#ifndef FUNCTION_H
#define FUNCTION_H

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "AST.h"
#include "Value.h"
#include "Statement.h"

class Function {
 private:
  // Variables that were captured when the function def was evaluated:
  std::map<std::string, Value> my_scope{};
  // The code to run when this function is called:
//   std::weak_ptr<AST> code;
  Block* code;
  std::string name;
  std::vector<std::string> arguments;

 public:
  Function(std::string name, std::vector<std::string> arguments, std::map<std::string, Value> scope, Block* block);
  ~Function();
  // std::shared_ptr<AST> test(std::weak_ptr<AST> weak);
  std::map<std::string, Value> get_scope();
  std::vector<std::string> get_args();
  Block* get_code();
  Value run_code(std::map<std::string, Value> scope);
};

#endif