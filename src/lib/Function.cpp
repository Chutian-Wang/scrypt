#include "Function.h"

#include "Errors.h"

Function::Function(std::weak_ptr<AST> functionAST) : funct(functionAST) {}

std::map<std::string, Value> Function::getScope() const {
    // a copy of the entire scope
  return my_scope;
}

const std::map<std::string, Value>& Function::getLocalScope() const {
    // reference to the local scope
    return my_scope;
}

void Function::setScopeStack(std::stack<std::shared_ptr<Function>>& stack) {
    scopeStack = stack;
}

void Function::addVariable(const std::string& name, const Value& value) {
  my_scope[name] = value;
}

void Function::setRet(const Value& value) { my_scope["return"] = value; }

Value Function::getVariable(const std::string& name) const {
  auto it = my_scope.find(name);
  if (it != my_scope.end()) {
    return it->second;
  } else {
    // Variable not found
    throw InvalidAssignee();
  }
}