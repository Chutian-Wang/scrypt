#include <iostream>
#include <sstream>
#include <vector>

#include "lib/AST.h"
#include "lib/Errors.h"
#include "lib/Lexer.h"
#include "lib/Statement.h"

int main() {
  try {
    Lexer lexer;
    lexer.tokenize(std::cin);

    std::vector<Token> tokens = lexer.get_tokens();
    std::unique_ptr<Block> program = Block::parse_block(tokens);

    program->run();
  } catch (const ScryptError &err) {
    ScryptError::handle(std::cout, err);
  }
  return 0;
}