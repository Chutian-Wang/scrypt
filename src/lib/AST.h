#ifndef AST_H
#define AST_H

#include <memory>
#include <string>
#include <vector>

#include "Token.h"

struct Value;
class Function; 

extern std::map<std::string, Value> symbols;

/**
 * This is the base class for all Nodes.
 */
class AST {
 public:
  virtual ~AST(){};
  /**
   * This function handles infix parsing.
   */
  static std::shared_ptr<AST> parse_infix(
      std::vector<Token>::const_iterator &head, int min_p);

  /**
   * This function handles primary parsing.
   */
  static std::shared_ptr<AST> parse_primary(
      std::vector<Token>::const_iterator &head);

  /**
   * This function handles function call parsing.
   */
  static std::vector<std::shared_ptr<AST>> parse_call(
      std::vector<Token>::const_iterator &head, TokenType end);

  /**
   * Function for parse functions to get node tokens
   */
  virtual const Token &get_token() const = 0;

  /**
   * User function to evaluate the subtree:
   * This function will revert identifiers to their previous
   * values if evaluation fails
   */
  virtual Value eval(std::shared_ptr<Function> currentFunc) const = 0;

  /**
   * Internal function to evaluate the subtree:
   * This function will NOT revert identifiers to their previous
   * values if evaluation fails
   */
  virtual Value __eval() const = 0;

  /**
   * This function pushes the subtree's infix form into oss
   */
  virtual void get_infix(std::ostream &oss) const = 0;

  /**
   * This function consumes the target Token if head == target
   * o.w. throws an unexpected token error
   */
  static void consume(std::vector<Token>::const_iterator &head,
                      TokenType target);

  /************************** S-parser *************************
  **                         DEPRECIATED                      **
  **************************************************************
   // Top level multiple line S expression parser.
   // Call this function to construct multiple expressions
   // from standard input.
   static std::vector<std::shared_ptr<AST>> parse_S_multiple(
       const std::vector<Token> &tokens);
   // Internal S expression parser, expects one line.
   static std::shared_ptr<AST> parse_S_top(
       std::vector<Token>::const_iterator &head);
   // Internal S expression base level recursive parser.
   static std::shared_ptr<AST> parse_S(std::vector<Token>::const_iterator
 &head);
 **************************************************************/

  /*************************************************************
  **                    Infix Scrypt Parser                   **
  *************************************************************/

  /**
   * Top level single expression infix parser.
   * Call this function to construct a top level AST from
   * infix expressions.
   */
  // static std::shared_ptr<AST> parse_infix(const std::vector<Token> &tokens);
  /**
   * Infix sub-expression parser. This function parses everything
   * enclosed by a pair of LPAREN and RPAREN or a free-standing
   * expression if properlly called.
   */
  //   static std::shared_ptr<AST> parse_infix(
  //       std::vector<Token>::const_iterator &head);

  /**
   * Base level operator precedance recursive parser.
   */
  //   static std::shared_ptr<AST> parse_infix(
  //       std::vector<Token>::const_iterator &head, std::shared_ptr<AST> lhs,
  //       int min_p);

  /**
   * Function for parse_S functions to get the static
   * validity of a node, if an Identifier is not assigned
   * it is still deemed valid in static analysis.
   * Deserted due to depreciation of S expression evaluation
   */
  // virtual bool is_legal() const = 0;
};

#endif
