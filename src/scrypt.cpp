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
    auto tokens = lexer.get_tokens();
    std::vector<Token>::const_iterator head = tokens.begin();
    std::unique_ptr<Block> program = Block::parse_block(tokens, head);

    program->run();
  } catch (const ScryptError &err) {
    ScryptError::handle(std::cout, err);
    return 1;
  }
  return 0;
}