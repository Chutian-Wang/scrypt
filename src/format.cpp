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
    if (tokens.empty()) {
      return 1;
    }
    std::unique_ptr<Block> program = Block::parse_block(tokens);
    program->print(std::cout, 0);
  } catch (const ScryptError &err) {
    return ScryptError::handle(std::cout, err);
  }
  return 0;
}