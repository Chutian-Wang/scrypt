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
    // This global map tracks the variables
    // declare as external in other files where needed
    std::map<std::string, Value> symbols{};
    std::unique_ptr<Block> program = Block::parse_block(tokens);
    program->run(symbols);
  } catch (Value& value) {
    std::cout << "Runtime error: unexpected return.\n";
    return 3;
  } catch (const ScryptError& err) {
    return ScryptError::handle(std::cout, err);
  }
  return 0;
}