#include "lib/demo_lib.h"
#include <iomanip>
#include <vector>
#include "lib/token.h"
#include "lib/lexer.h"
#include <sstream>

int main() {
    std::cout << std::stod(".2") << std::endl;
    std::string str, token;
    std::getline(std::cin, str);
    std::istringstream stream(str);
    while(stream >> token){
        std::cout << token << std::endl;
    }

    // std::string line;
    // std::getline(std::cin, line);
    // //std::istringstream iss(line);
    // try{
    //     Lexer lexer(line);
    //     lexer.tokenize();

    //     std::vector<Token> tokens;
    //     tokens = lexer.getTokens();

    //     for(auto token: tokens){
    //         std::cout << std::setw(3) << token.row;
    //         std::cout << std::setw(4) << token.column << "  ";
    //         std::cout << token.chr << std::endl;
    //     }
    // }
    // catch(Token error){
    //     std::cout << "Syntax error on line " << error.row << " column " << error.column << ".\n";
    //     exit(1);
    // }
    return 0;
}
