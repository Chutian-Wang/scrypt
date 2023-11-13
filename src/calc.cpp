#include <iomanip>
#include <iostream>
#include <sstream>

#include "lib/AST.h"
#include "lib/Errors.h"
#include "lib/Lexer.h"
#include "lib/Value.h"

int main() {
  std::shared_ptr<AST> parser;
  std::string line;
  while (std::getline(std::cin, line)) {
    std::cout << "line0: " << line << std::endl;
    try {
      auto iss = std::istringstream(line);
      Lexer lexer;
      lexer.tokenize(iss);
      parser = AST::parse_infix(lexer.get_tokens());
      parser->get_infix(std::cout);
      std::cout << std::endl;
      Value result = parser->eval();
      std::cout << result << std::endl;
    } catch (const InvalidAssignee &err) {
        std::cout << line << std::endl;
        InvalidAssignee::handle(std::cout, err);
    } catch (const ScryptError &err) {
      ScryptError::handle(std::cout, err);
    }
  }
  return 0;
}
