#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>
#include <memory>
#include <vector>

#include "AST.h"

class Function{
  private:
    std::map<std::string, Value> my_scope{};
    std::map<std::string, Value> parent_scope{};
    std::weak_ptr<AST> funct;
    
  public:
    std::map<std::string, Value> get_scope(std::string scope);
}

#endif