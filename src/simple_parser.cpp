//----------------------------------------------------------------------
// FILE: simple_parser.cpp
// DATE: CPSC 326, Spring 2023
// AUTH: Jonathan Smoley
// DESC: Basic interface for our Simple Parser (syntax checker)
//----------------------------------------------------------------------

#include "simple_parser.h"

SimpleParser::SimpleParser(const Lexer &a_lexer)
    : lexer{a_lexer}
{
}

void SimpleParser::advance()
{
  curr_token = lexer.next_token();
}

void SimpleParser::eat(TokenType t, const std::string &msg)
{
  if (!match(t))
    error(msg);
  advance();
}

bool SimpleParser::match(TokenType t)
{
  return curr_token.type() == t;
}

bool SimpleParser::match(std::initializer_list<TokenType> types)
{
  for (auto t : types)
    if (match(t))
      return true;
  return false;
}

void SimpleParser::error(const std::string &msg)
{
  std::string s = msg + " found '" + curr_token.lexeme() + "' ";
  s += "at line " + std::to_string(curr_token.line()) + ", ";
  s += "column " + std::to_string(curr_token.column());
  throw MyPLException::ParserError(s);
}

bool SimpleParser::bin_op()
{
  return match({TokenType::PLUS, TokenType::MINUS, TokenType::TIMES,
                TokenType::DIVIDE, TokenType::AND, TokenType::OR, TokenType::EQUAL,
                TokenType::LESS, TokenType::GREATER, TokenType::LESS_EQ,
                TokenType::GREATER_EQ, TokenType::NOT_EQUAL});
}

void SimpleParser::parse()
{
  advance();
  while (!match(TokenType::EOS))
  {
    if (match(TokenType::STRUCT))
      struct_def();
    else
      fun_def();
  }
  eat(TokenType::EOS, "expecting end-of-file");
}

void SimpleParser::struct_def()
{
  eat(TokenType::STRUCT, "expecting 'struct'");
  eat(TokenType::ID, "expecting identifier");
  eat(TokenType::LBRACE, "expecting '{'");
  fields();
  eat(TokenType::RBRACE, "expecting '}'");
}

void SimpleParser::fields()
{
  if (base_type() || match({TokenType::ID, TokenType::ARRAY, TokenType::DICT}))
  {
    data_type();
    eat(TokenType::ID, "expecting identifier");
    while (match(TokenType::COMMA))
    {
      eat(TokenType::COMMA, "expecting ','");
      data_type();
      eat(TokenType::ID, "expecting identifier");
    }
  }
}

void SimpleParser::fun_def()
{
  if (match(TokenType::VOID_TYPE))
    eat(TokenType::VOID_TYPE, "expecting 'void'");
  else
    data_type();
  eat(TokenType::ID, "expecting identifier");
  eat(TokenType::LPAREN, "expecting '('");
  params();
  eat(TokenType::RPAREN, "expecting ')'");
  eat(TokenType::LBRACE, "expecting '{'");
  while (!match(TokenType::RBRACE))
    stmt();
  eat(TokenType::RBRACE, "expecting '}'");
}

void SimpleParser::params()
{
  if (base_type() || match({TokenType::ID, TokenType::ARRAY, TokenType::DICT}))
  {
    data_type();
    eat(TokenType::ID, "expecting identifier");
    while (match(TokenType::COMMA))
    {
      eat(TokenType::COMMA, "expecting ','");
      data_type();
      eat(TokenType::ID, "expecting identifier");
    }
  }
}

void SimpleParser::data_type()
{
  if (base_type())
  {
    eat(curr_token.type(), "expecting base type");
    if (!match(TokenType::ID))
      error("expecting identifier after base type");
  }
  else if (match(TokenType::ID))
    eat(TokenType::ID, "expecting identifier");
  else if (match(TokenType::ARRAY))
  {
    eat(TokenType::ARRAY, "expecting 'array'");
    if (base_type())
      eat(curr_token.type(), "expecting base type");
    else if (match(TokenType::ID))
      eat(TokenType::ID, "expecting identifier");
  }
  else if (match(TokenType::DICT))
  {
    advance();
    if (!match({TokenType::ID, TokenType::STRING_TYPE, 
                TokenType::INT_TYPE, TokenType::CHAR_TYPE}))
      error("expecting key type");
    advance();
    if (!base_type() && !match({TokenType::ID, TokenType::ARRAY}))
      error("expecting value type");
    advance();
  }
}

bool SimpleParser::base_type()
{
  return match({TokenType::INT_TYPE, TokenType::DOUBLE_TYPE,
                TokenType::BOOL_TYPE, TokenType::CHAR_TYPE,
                TokenType::STRING_TYPE});
}

void SimpleParser::stmt()
{
  if (match(TokenType::IF))
    if_stmt();
  else if (match(TokenType::WHILE))
    while_stmt();
  else if (match(TokenType::FOR))
    for_stmt();
  else if (match(TokenType::RETURN))
    ret_stmt();
  else if (match({TokenType::ARRAY, TokenType::DICT}) || base_type())
    vdecl_stmt();
  else if (match(TokenType::ID))
  {
    eat(TokenType::ID, "expecting identifier");
    // assign_stmt
    if (match({TokenType::DOT, TokenType::LBRACKET}))
      assign_stmt();
    // call_expr
    else if (match(TokenType::LPAREN))
      call_expr();
    // vdecl_stmt
    else
      vdecl_stmt();
  }
  else
    error("expecting statement");
}

void SimpleParser::vdecl_stmt()
{
  data_type();
  if (!match(TokenType::ASSIGN))
    eat(TokenType::ID, "expecting identifier");
  eat(TokenType::ASSIGN, "expecting '='");
  expr();
}

void SimpleParser::assign_stmt()
{
  lvalue();
  eat(TokenType::ASSIGN, "expecting '='");
  expr();
}

void SimpleParser::lvalue()
{
  if (match(TokenType::ID))
    eat(TokenType::ID, "expecting identifier");
  while (match({TokenType::DOT, TokenType::LBRACKET}))
  {
    if (match(TokenType::DOT))
    {
      eat(TokenType::DOT, "expecting '.'");
      eat(TokenType::ID, "expecting identifier");
    }
    else if (match(TokenType::LBRACKET))
    {
      eat(TokenType::LBRACKET, "expecting '['");
      expr();
      eat(TokenType::RBRACKET, "expecting ']'");
    }
  }
}

void SimpleParser::if_stmt()
{
  eat(TokenType::IF, "expecting 'if'");
  eat(TokenType::LPAREN, "expecting '('");
  expr();
  eat(TokenType::RPAREN, "expecting ')'");
  eat(TokenType::LBRACE, "expecting '{'");
  while (!match(TokenType::RBRACE))
    stmt();
  eat(TokenType::RBRACE, "expecting '}'");
  if_stmt_t();
}

void SimpleParser::if_stmt_t()
{
  if (match(TokenType::ELSEIF))
  {
    eat(TokenType::ELSEIF, "expecting 'elseif'");
    expr();
    eat(TokenType::LBRACE, "expecting '{'");
    while (!match(TokenType::RBRACE))
      stmt();
    eat(TokenType::RBRACE, "expecting '}'");
    if_stmt_t();
  }
  else if (match(TokenType::ELSE))
  {
    eat(TokenType::ELSE, "expecting 'else'");
    eat(TokenType::LBRACE, "expecting '{'");
    while (!match(TokenType::RBRACE))
      stmt();
    eat(TokenType::RBRACE, "expecting '}'");
  }
}

void SimpleParser::while_stmt()
{
  eat(TokenType::WHILE, "expecting 'while'");
  expr();
  eat(TokenType::LBRACE, "expecting '{'");
  while (!match(TokenType::RBRACE))
    stmt();
  eat(TokenType::RBRACE, "expecting '}'");
}

void SimpleParser::for_stmt()
{
  eat(TokenType::FOR, "expecting 'for'");
  eat(TokenType::LPAREN, "expecting '('");
  vdecl_stmt();
  eat(TokenType::SEMICOLON, "expecting ';'");
  expr();
  eat(TokenType::SEMICOLON, "expecting ';'");
  assign_stmt();
  eat(TokenType::RPAREN, "expecting ')'");
  eat(TokenType::LBRACE, "expecting '{'");
  if (!match(TokenType::RBRACE))
  {
    while (!match(TokenType::RBRACE))
      stmt();
  }
  else 
    error("expecting stmt in loop");
  eat(TokenType::RBRACE, "expecting '}'");
}

void SimpleParser::call_expr()
{
  eat(TokenType::LPAREN, "expecting '('");
  if (base_rvalue() || match({TokenType::NULL_VAL, 
          TokenType::NEW, TokenType::ID}))
  {
    expr();
    while (match(TokenType::COMMA))
    {
      eat(TokenType::COMMA, "expecting ','");
      expr();
    }
  }
  eat(TokenType::RPAREN, "expecting ')'");
}

void SimpleParser::ret_stmt()
{
  eat(TokenType::RETURN, "expecting 'return'");
  expr();
}

void SimpleParser::expr()
{
  if (match(TokenType::NOT))
  {
    eat(TokenType::NOT, "expecting 'NOT'");
    expr();
  }
  else if (match(TokenType::LPAREN))
  {
    eat(TokenType::LPAREN, "expecting '('");
    expr();
    eat(TokenType::RPAREN, "expecting ')'");
  }
  else if (base_rvalue() || match({TokenType::NULL_VAL, 
          TokenType::NEW, TokenType::ID}))
    rvalue();
  else
    error("expecting expression");

  if (bin_op())
  {
    eat(curr_token.type(), "expecting binary operation");
    expr();
  }
}

void SimpleParser::rvalue()
{
  if (match(TokenType::NULL_VAL))
    eat(TokenType::NULL_VAL, "expecting 'null'");
  else if (base_rvalue())
    eat(curr_token.type(), "expecting base value");
  else if (match(TokenType::NEW))
    new_rvalue();
  else if (match(TokenType::ID))
  {
    eat(TokenType::ID, "expecting identifier");
    if (match(TokenType::LPAREN))
      call_expr();
    else if (match({TokenType::DOT, TokenType::LBRACKET}))
      var_rvalue();
  }
}

void SimpleParser::new_rvalue()
{
  eat(TokenType::NEW, "expecting 'new'");
  if (match(TokenType::ID))
  {
    eat(TokenType::ID, "expecting identifier");
    if (match(TokenType::LBRACKET))
    {
      eat(TokenType::LBRACKET, "expecting '['");
      expr();
      eat(TokenType::RBRACKET, "expecting ']'");
    }
  }
  else if (match(TokenType::DICT))
  {
    advance();
    eat(TokenType::LBRACE, "expecting start of dict decl");
    if (!match({TokenType::ID, TokenType::STRING_TYPE, 
                TokenType::INT_TYPE, TokenType::CHAR_TYPE}))
      error("expecting valid key type");
    advance();
    eat(TokenType::COMMA, "expecting comma");
    if (!base_type() && !match({TokenType::ID, TokenType::ARRAY}))
      error("expecting valid value type");
    advance();
    eat(TokenType::RBRACE, "expecting end of dict decl");
  }
  else
  {
    if (base_type())
    {
      eat(curr_token.type(), "expecting base type");
      eat(TokenType::LBRACKET, "expecting '['");
      expr();
      eat(TokenType::RBRACKET, "expecting ']'");
    }
  }
}

bool SimpleParser::base_rvalue()
{
  return match({TokenType::INT_VAL, TokenType::DOUBLE_VAL,
                TokenType::BOOL_VAL, TokenType::CHAR_VAL,
                TokenType::STRING_VAL});
}

void SimpleParser::var_rvalue()
{
  while (match({TokenType::DOT, TokenType::LBRACKET}))
  {
    if (match(TokenType::DOT))
    {
      eat(TokenType::DOT, "expecting '.'");
      eat(TokenType::ID, "expecting identifier");
    }
    else if (match(TokenType::LBRACKET))
    {
      eat(TokenType::LBRACKET, "expecting '['");
      expr();
      eat(TokenType::RBRACKET, "expecting ']'");
    }
  }
}
