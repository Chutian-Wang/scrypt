#include "Function.h"

#include "Errors.h"

// #include <memory>
// #include <map>

Function::Function(std::weak_ptr<AST> functionAST) : funct(functionAST) {}

std::map<std::string, Value> Function::getScope() const {
  return my_scope;
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