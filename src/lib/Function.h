#ifndef FUNCTION_H
#define FUNCTION_H

#include <memory>
#include <string>
#include <vector>
#include <stack>

#include "AST.h"
#include "Value.h"

class Function {
 private:
  std::map<std::string, Value> my_scope{};
  std::stack<std::shared_ptr<Function>> scopeStack;
  std::weak_ptr<AST> funct;

 public:
  Function(){};
  Function(std::weak_ptr<AST> functionAST);
  std::map<std::string, Value> getScope() const;
  const std::map<std::string, Value>& getLocalScope() const;
  
  void setScopeStack(std::stack<std::shared_ptr<Function>>& stack);
  void addVariable(const std::string& name, const Value& value);
  void setRet(const Value& value);
  Value getVariable(const std::string& name) const;
};

#endif