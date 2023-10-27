#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>

#include "lib/AST.h"
#include "lib/Errors.h"
#include "lib/Lexer.h"

int main() {
  std::vector<std::shared_ptr<AST>> expressions;
  int return_code = 0;
  try {
    Lexer lexer;
    lexer.tokenize(std::cin);
    // std::string input = "(= a 3)";
    // std::istringstream iss(input);
    // lexer.tokenize(iss);
    auto tokens = lexer.get_tokens();
    expressions = AST::parse_S_multiple(tokens);
    for (auto expr : expressions) {
      std::ostringstream oss;
      expr->get_infix(oss);
      std::cout << oss.str() << '\n';
      try {
        std::cout << expr->eval() << std::endl;
      } catch (const ScryptError &err) {
        return_code = ScryptError::handle(std::cout, err);
      }
    }
  } catch (const ScryptError &err) {
    return ScryptError::handle(std::cout, err);
  }

  return return_code;
}