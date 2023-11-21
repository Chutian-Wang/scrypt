#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>

#include "lib/AST.h"
#include "lib/Errors.h"
#include "lib/Lexer.h"
#include "lib/Function.h"

int main() {
  std::vector<std::shared_ptr<AST>> expressions;
  int return_code = 0;
  Function globalScope;
  std::stack<std::shared_ptr<Function>> scopeStack;
  globalScope.setScopeStack(scopeStack);
  try {
    Lexer lexer;
    lexer.tokenize(std::cin);
    auto tokens = lexer.get_tokens();
    expressions = AST::parse_S_multiple(tokens);
    for (auto expr : expressions) {
      std::ostringstream oss;
      expr->get_infix(oss);
      std::cout << oss.str() << '\n';
      try {
        std::cout << expr->eval(std::make_shared<Function>(globalScope)) << std::endl;
      } catch (const ScryptError &err) {
        return_code = ScryptError::handle(std::cout, err);
      }
    }
  } catch (const ScryptError &err) {
    return ScryptError::handle(std::cout, err);
  }

  return return_code;
}