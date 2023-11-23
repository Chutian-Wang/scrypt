#include "Function.h"
#include "Value.h"

#include <memory>
#include <string>
#include <vector>
#include <map>

Function::Function(std::string name, std::vector<std::string> arguments, std::map<std::string, Value> scope, Block* block){
    this->name = name;
    this->arguments = arguments;
    this->my_scope = scope;
    this->code = block;
}

Function::~Function(){}

std::map<std::string, Value> Function::get_scope(){
    return this->my_scope;
}

std::vector<std::string> Function::get_args(){
    return this->arguments;
}

Value Function::run_code(std::map<std::string, Value> scope){
    // handles unexpect return
    try {
        code->run(scope);
        return Value(nullptr);
    }
    catch(Value& value) {
        return value;
    }
}
