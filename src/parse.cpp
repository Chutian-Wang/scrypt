#include <iomanip>
#include <iostream>
#include <sstream>

#include "lib/AST.h"
#include "lib/Errors.h"
#include "lib/Lexer.h"

int main() {
  static AST *parser;
  try {
    Lexer lexer;
    lexer.tokenize(std::cin);

    auto tokens = lexer.get_tokens();
    parser = AST::parse_S(tokens);
    std::ostringstream oss;
    parser->get_infix_S(oss);
    std::cout << oss.str() << '\n';
  } catch (const ScryptError &err) {
    std::cout << err.what() << std::endl;
    return SYNTAX_ERR;
  }

  try {
    std::cout << parser->eval() << std::endl;
    delete parser;
  } catch (const ScryptError &err) {
    return ScryptError::handle(std::cout, err);
  }

  return 0;
}