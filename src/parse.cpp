#include <iomanip>
#include <iostream>
#include <sstream>

#include "lib/AST.h"
#include "lib/Errors.h"
#include "lib/Lexer.h"

int main() {
  std::vector<AST *> expressions;
  try {
    Lexer lexer;
    lexer.tokenize(std::cin);
    auto tokens = lexer.get_tokens();
    expressions = AST::parse_S_multiple(tokens);
    for (auto expr : expressions) {
      std::ostringstream oss;
      expr->get_infix_S(oss);
      std::cout << oss.str() << '\n';
      try {
        std::cout << expr->eval() << std::endl;
      } catch (const ScryptError &err) {
        std::cout << err.what() << std::endl;
      }
      delete expr;
    }
  } catch (const ScryptError &err) {
    return ScryptError::handle(std::cout, err);
  }
  return 0;
}