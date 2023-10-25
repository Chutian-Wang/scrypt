#include <iomanip>
#include <iostream>
#include <sstream>

#include "lib/AST.h"
#include "lib/Errors.h"
#include "lib/Lexer.h"

int main() {
  std::vector<AST*> expressions;
  // static AST *parser;
  try {
    Lexer lexer;
    lexer.tokenize(std::cin);
    // std::string input = "(* 2 20) (+ 1 2)";
    // std::istringstream iss(input);
    // lexer.tokenize(iss);
    auto tokens = lexer.get_tokens();
    for (auto token : tokens) {
      std::cout << std::setw(4) << token.row;
      std::cout << std::setw(5) << token.column << "  ";
      std::cout << token.text << std::endl;
    }
    expressions = AST::parse_S_multiple(tokens);
    for (auto expr: expressions) {
      std::ostringstream oss;
      expr->get_infix_S(oss);
      std::cout << oss.str() << '\n';
      std::cout << expr->eval() << std::endl;
      delete expr;
    }
  } catch (const ScryptError &err) {
    return ScryptError::handle(std::cout, err);
  }
  return 0;
}