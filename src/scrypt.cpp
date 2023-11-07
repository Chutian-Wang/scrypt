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
    std::string input;
    std::string line;
    while (std::getline(std::cin, line)) {
      input += line + '\n';
      std::istringstream iss(line);
      lexer.tokenize(iss);
    }

    std::vector<Token> tokens = lexer.get_tokens();
    std::unique_ptr<Block> program = Block::parse_block(tokens);

    program->run();
  } catch (const ScryptError &err) {
    ScryptError::handle(std::cerr, err);
    return 1;
  }
  return 0;
}