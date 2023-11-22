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
    try {
      auto iss = std::istringstream(line);
      Lexer lexer;
      lexer.tokenize(iss);
      std::vector<Token> tokens = lexer.get_tokens();
      std::vector<Token>::const_iterator head = tokens.begin();
      parser = AST::parse_infix(head, 0);
      parser->get_infix(std::cout);
      std::cout << std::endl;
      std::map<std::string, Value> symbols{};
      Value result = parser->eval(symbols);
      std::cout << result << std::endl;
    } catch (const ScryptError &err) {
      ScryptError::handle(std::cout, err);
    }
  }
  return 0;
}
