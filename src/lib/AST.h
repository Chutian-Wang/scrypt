#ifndef AST_H
#define AST_H

#include <memory>
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
  static std::vector<std::shared_ptr<AST>> parse_S_multiple(
      const std::vector<Token> &tokens);
  static std::shared_ptr<AST> parse_S_top(
      std::vector<Token>::const_iterator &head);
  static std::shared_ptr<AST> parse_S(std::vector<Token>::const_iterator &head);

  /**
   * These are the new parser constructor that supports
   * infix form. TODO.
   */
  static std::shared_ptr<AST> parse_infix(const std::vector<Token> &tokens);
  static std::shared_ptr<AST> parse_infix(
      std::vector<Token>::const_iterator &head);
  static std::shared_ptr<AST> parse_infix(
      std::vector<Token>::const_iterator &head, std::shared_ptr<AST> lhs,
      int min_p);
  static std::shared_ptr<AST> parse_primary(const Token &tok);

  virtual ~AST(){};

  // Function for parse functions to get node tokens
  virtual const Token &get_token() const = 0;
  // Function for parse_S functions to get the static
  // validity of a node, if an Identifier is not assigned
  // it is still deemed valid in static analysis.
  virtual bool is_legal() const = 0;
  // User function to evaluate the subtree:
  virtual double eval() const = 0;
  // Internal function to evaluate the subtree:
  virtual double __eval() const = 0;
  // These function pushes the subtree's infix form into oss
  virtual void get_infix(std::ostream &oss) const = 0;
};

#endif
