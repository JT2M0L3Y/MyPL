//----------------------------------------------------------------------
// FILE: lexer.cpp
// DATE: CPSC 326, Spring 2023
// NAME: Jonathan Smoley
// DESC: Lexical Analaysis for MyPL
//----------------------------------------------------------------------

#include "lexer.h"

using namespace std;

Lexer::Lexer(istream &input_stream)
    : input{input_stream}, column{0}, line{1}
{
}

char Lexer::read()
{
  ++column;
  return input.get();
}

char Lexer::peek()
{
  return input.peek();
}

bool Lexer::match(char c, char lexeme)
{
  return c == lexeme;
}

void Lexer::error(const string &msg, int line, int column) const
{
  throw MyPLException::LexerError(msg + " at line " + to_string(line) +
                                  ", column " + to_string(column));
}

Token Lexer::next_token()
{
  char ch = read();

  // check newline
  while (match(ch, '\n'))
  {
    ch = read();
    ++line;
    column = 1;
  }

  // read all whitespace
  if (isspace(ch))
  {
    while (isspace(ch) || match(ch, '\n'))
    {
      ch = read();

      // check newline
      if (match(ch, '\n'))
      {
        ++line;
        column = 1;
      }
    }
  }

  // read comments
  if (match(ch, '#'))
  {
    while (match(ch, '#'))
    {
      while (!match(ch, '\n') && !match(ch, EOF))
      {
        ch = read();
      }
      // check end of file
      if (match(ch, EOF))
      {
        return Token(TokenType::EOS, "end-of-stream", line, column);
      }
      ch = read();
      column = 1;
      ++line;

      if (isspace(ch))
      {
        while (isspace(ch) || match(ch, '\n'))
        {
          // check newline
          if (match(ch, '\n'))
          {
            ++line;
            column = 0;
          }
          ch = read();
        }
      }
    }
  }

  // check for EOF
  if (match(ch, EOF))
  {
    return Token(TokenType::EOS, "end-of-stream", line, column);
  }

  // check for single character tokens (arithmetic, punctuation, etc.)
  switch (ch)
  {
  case '.':
    return Token(TokenType::DOT, ".", line, column);
  case ',':
    return Token(TokenType::COMMA, ",", line, column);
  case '(':
    return Token(TokenType::LPAREN, "(", line, column);
  case ')':
    return Token(TokenType::RPAREN, ")", line, column);
  case '[':
    return Token(TokenType::LBRACKET, "[", line, column);
  case ']':
    return Token(TokenType::RBRACKET, "]", line, column);
  case ';':
    return Token(TokenType::SEMICOLON, ";", line, column);
  case '{':
    return Token(TokenType::LBRACE, "{", line, column);
  case '}':
    return Token(TokenType::RBRACE, "}", line, column);
  case '+':
    return Token(TokenType::PLUS, "+", line, column);
  case '-':
    return Token(TokenType::MINUS, "-", line, column);
  case '*':
    return Token(TokenType::TIMES, "*", line, column);
  case '/':
    return Token(TokenType::DIVIDE, "/", line, column);
  }

  // check for basic comparators (two-character tokens)
  switch (ch)
  {
  case '<':
    if (match(peek(), '='))
    {
      read();
      return Token(TokenType::LESS_EQ, "<=", line, column - 1);
      // return Token(TokenType::LESS_EQ, "<=", line, column - 2);
    }
    // return Token(TokenType::LESS, "<", line, column - 1);
    return Token(TokenType::LESS, "<", line, column);
  case '>':
    if (match(peek(), '='))
    {
      read();
      // return Token(TokenType::GREATER_EQ, ">=", line, column - 2);
      return Token(TokenType::GREATER_EQ, ">=", line, column - 1);
    }
    // return Token(TokenType::GREATER, ">", line, column - 1);
    return Token(TokenType::GREATER, ">", line, column);
  case '!':
    if (match(peek(), '='))
    {
      read();
      // return Token(TokenType::NOT_EQUAL, "!=", line, column - 2);
      return Token(TokenType::NOT_EQUAL, "!=", line, column - 1);
    }
    error("expecting '!=' found '" + string(1, ch) + string(1, peek()) + "'", line, column);
  case '=':
    if (match(peek(), '='))
    {
      read();
      // return Token(TokenType::EQUAL, "==", line, column - 2);
      return Token(TokenType::EQUAL, "==", line, column - 1);
    }
    // return Token(TokenType::ASSIGN, "=", line, column - 1);
    return Token(TokenType::ASSIGN, "=", line, column);
  }

  // check for character values (tricky cases too like '\n')
  if (match(ch, '\''))
  {
    if (match(peek(), '\''))
    {
      // empty character
      error("empty character", line, column + 1);
    }
    else
    {
      ch = read();

      if (match(peek(), '\''))
      {
        // check newline
        if (match(ch, '\n'))
        {
          error("found end-of-line in character", line, column);
        }
        else if (match(ch, '\t'))
        {
          error("found tab in character", line, column);
        }

        // normal character
        read();
        return Token(TokenType::CHAR_VAL, string(1, ch), line, column - 2);
      }
      else if (match(ch, '\\'))
      {
        // newlines, tabs, etc.
        ch = read();

        if (match(ch, 'n'))
        {
          if (match(peek(), '\''))
          {
            read();
            return Token(TokenType::CHAR_VAL, "\\n", line, column - 3);
          }
        }
        else if (match(ch, 't'))
        {
          if (match(peek(), '\''))
          {
            read();
            return Token(TokenType::CHAR_VAL, "\\t", line, column - 3);
          }
        }
      }
      else
      {
        if (match(ch, EOF))
        {
          error("found end-of-file in character", line, column);
        }
        else
        {
          ch = read();
          error("expecting ' found " + string(1, ch), line, column);
        }
      }
    }
  }

  // check for string values
  else if (match(ch, '\"'))
  {
    ch = read();
    string result = "";

    while (!match(ch, '\"'))
    {
      if (match(ch, EOF))
      {
        error("found end-of-file in string", line, column + result.length());
      }
      if (match(ch, '\n'))
      {
        error("found end-of-line in string", line, column);
      }

      result += ch;
      ch = read();
    }

    int offset = result.length() + 1;
    return Token(TokenType::STRING_VAL, result, line, column - offset);
  }

  // check for integer & double values
  else if (isdigit(ch))
  {
    string result = "";
    bool isDouble = false;

    while (peek() == '.' || isdigit(peek()))
    {
      if (peek() == '.')
      {
        // check for existing double
        if (!isDouble)
        {
          isDouble = true;
          result += ch;
        }
        else
        {
          // already found a double
          break;
        }
      }
      else
      {
        result += ch;
      }
      ch = read();
    }

    result += ch;
    int offset = result.length() - 1;

    if (isDouble)
    {
      if (!isdigit(ch))
      {
        read();
        error("missing digit in '" + result + "'", line, column);
      }
      return Token(TokenType::DOUBLE_VAL, result, line, column - offset);
    }
    else
    {
      if (result[0] == '0' && result.length() > 1)
      {
        error("leading zero in number", line, column - offset);
      }
      return Token(TokenType::INT_VAL, result, line, column - offset);
    }
  }

  // check for reserved words
  string result = "";

  while (isalpha(peek()))
  {
    result += ch;
    ch = read();
  }
  if (!isalpha(ch))
  {
    error("unexpected character '" + string(1, ch) + "'", line, column);
  }

  result += ch;
  int offset = result.length() - 1;

  // booleans
  if (result == "true")
  {
    return Token(TokenType::BOOL_VAL, result, line, column - offset);
  }
  else if (result == "false")
  {
    return Token(TokenType::BOOL_VAL, result, line, column - offset);
  }

  // null
  else if (result == "null")
  {
    return Token(TokenType::NULL_VAL, result, line, column - offset);
  }

  // data types
  else if (result == "int")
  {
    return Token(TokenType::INT_TYPE, result, line, column - offset);
  }
  else if (result == "double")
  {
    return Token(TokenType::DOUBLE_TYPE, result, line, column - offset);
  }
  else if (result == "bool")
  {
    return Token(TokenType::BOOL_TYPE, result, line, column - offset);
  }
  else if (result == "string")
  {
    return Token(TokenType::STRING_TYPE, result, line, column - offset);
  }
  else if (result == "char")
  {
    return Token(TokenType::CHAR_TYPE, result, line, column - offset);
  }
  else if (result == "void")
  {
    return Token(TokenType::VOID_TYPE, result, line, column - offset);
  }

  // objects
  else if (result == "struct")
  {
    return Token(TokenType::STRUCT, result, line, column - offset);
  }
  else if (result == "array")
  {
    return Token(TokenType::ARRAY, result, line, column - offset);
  }

  // loops
  else if (result == "for")
  {
    return Token(TokenType::FOR, result, line, column - offset);
  }
  else if (result == "while")
  {
    return Token(TokenType::WHILE, result, line, column - offset);
  }

  // branches
  else if (result == "if")
  {
    return Token(TokenType::IF, result, line, column - offset);
  }
  else if (result == "elseif")
  {
    return Token(TokenType::ELSEIF, result, line, column - offset);
  }
  else if (result == "else")
  {
    return Token(TokenType::ELSE, result, line, column - offset);
  }

  // logical operators
  else if (result == "and")
  {
    return Token(TokenType::AND, result, line, column - offset);
  }
  else if (result == "or")
  {
    return Token(TokenType::OR, result, line, column - offset);
  }
  else if (result == "not")
  {
    return Token(TokenType::NOT, result, line, column - offset);
  }

  // others
  else if (result == "new")
  {
    return Token(TokenType::NEW, result, line, column - offset);
  }
  else if (result == "return")
  {
    return Token(TokenType::RETURN, result, line, column - offset);
  }

  // anything else has to be an identifier or invalid
  if (!(isalpha(peek()) || isdigit(peek()) || match(peek(), '_')))
  {
    return Token(TokenType::ID, result, line, column - offset);
  }
  // check nums, letters, and underscores
  else
  {
    ch = read();
    while (isalpha(peek()) || isdigit(peek()) || match(peek(), '_'))
    {
      result += ch;
      ch = read();
    }
  }

  result += ch;
  offset = result.length() - 1;

  return Token(TokenType::ID, result, line, column - offset);
}
