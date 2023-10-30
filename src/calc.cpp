#include <iomanip>
#include <iostream>
#include <sstream>
#include "lib/Value.h"
#include "lib/AST.h"
#include "lib/Errors.h"
#include "lib/Lexer.h"

int main() {
  std::shared_ptr<AST> parser;
  std::string line;
  while (std::getline(std::cin, line)) {
    try {
      auto iss = std::istringstream(line);
      Lexer lexer;
      lexer.tokenize(iss);
      parser = AST::parse_infix(lexer.get_tokens());
      parser->get_infix(std::cout);
      std::cout << std::endl;
      Value result = parser->eval();
      if (result.type == ValueType::BOOL){
        if (result._bool) {
          std::cout << "true" << std::endl;
        } else {
          std::cout << "false" << std::endl;
        }
        
      }
      if (result.type == ValueType::DOUBLE){
        std::cout << result._double << std::endl;
      }
    } catch (const ScryptError &err) {
      ScryptError::handle(std::cout, err);
    }
  }
  return 0;
}
