//----------------------------------------------------------------------
// FILE: lexer.h
// DATE: CPSC 326, Spring 2023
// NAME: Jonathan Smoley
// DESC: Lexical Analaysis for MyPL
//----------------------------------------------------------------------

#ifndef LEXER_H
#define LEXER_H

#include <istream>
#include <string>
#include "mypl_exception.h"
#include "token.h"

#include <cctype>
class Lexer
{
public:
  // Construct a new lexer from the given input stream
  Lexer(std::istream &input_stream);

  // Return the next available token in the input stream. Returns the
  // EOS (end of stream) token if no more tokens exist in the input
  // stream.
  Token next_token();

private:
  // input stream
  std::istream &input;

  // current line
  int line;

  // current column
  int column;

  // returns single character from input stream, advances stream, and
  // increments column number
  int read();

  // returns single character from input stream without advancing and
  // without incrementing column number
  int peek();

  // returns true if the given character matches the lexeme provided
  static constexpr bool match(int c, int lexeme) { return c == lexeme; }
};

#endif
