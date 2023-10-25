#ifndef AST_H
#define AST_H

#include <string>
#include <vector>

#include "Token.h"

/**
 * This is the base class for all Nodes.
 * The static parse_S functions construct an AST.
 */

class AST {
 public:
  /**
   * Function for parsing top level tree creation,
   * This function calls the explicit version with
   * head = tokens.begin()
   * These functions expect S-expression input form
   */
  static std::vector<AST*> parse_S_multiple(const std::vector<Token> &tokens);
  static AST *parse_S_short(std::vector<Token>::const_iterator &head);
  static AST *parse_S(std::vector<Token>::const_iterator &head);

  /**
   * These are the new parser constructor that supports
   * infix form. TODO.
   */
  static AST *parse_infix(const std::vector<Token> &tokens);
  static AST *parse_infix(const std::vector<Token> &tokens,
                          std::vector<Token>::const_iterator &head);

  virtual ~AST(){};

  // Function for parse functions to get node tokens
  virtual const Token &get_token() const = 0;
  // Function for parse functions to get validity of a node
  virtual bool is_legal() const = 0;
  // Function to evaluate the subtree:
  virtual double eval() const = 0;
  // These function pushes the subtree's infix form into oss
  virtual void get_infix_S(std::ostringstream &oss) const = 0;
  virtual void get_infix_infix(std::ostringstream &oss) const = 0;
};

#endif
