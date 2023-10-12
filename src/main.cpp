#include "lib/demo_lib.h"
#include <iomanip>
#include <vector>
#include "lib/token.h"
#include "lib/lexer.h"

int main() {
    std::string line;
    Lexer lexer;
    
    while(std::getline(std::cin, line)){
        if (line.size() == 0) break;
        lexer.tokenize(line);
    }
    
    std::vector<Token> tokens;
    tokens = lexer.getTokens();

    for(auto token: tokens){
        std::cout << std::setw(3) << token.row;
        std::cout << std::setw(4) << token.column << "  ";
        std::cout << token.chr;
    }
    return 0;
}