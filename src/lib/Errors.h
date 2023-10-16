#ifndef ERRORS_H
#define ERRORS_H

#include "Token.h"
#include <iostream>

/**
 * This header contains all of the possible errors in this
 * project. Please **DO NOT** inline any exit(), throw, or
 * equivalent error handling or use any error handling
 * function outside of the definitions in this header.
 * !!! These functions are not at all elegant ways to deal
 * with exceptions and error handling. This temporary
 * solution will be updated with better ones in the next
 * checkpoints !!!
*/

void syntax_err(Token tok);
void unexp_tok_err(Token tok);
void exp_tok_err(Token tok);
void div_by_zero_err();

#endif