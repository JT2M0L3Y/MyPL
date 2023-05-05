//----------------------------------------------------------------------
// FILE: ast_parser.cpp
// DATE: CPSC 326, Spring 2023
// AUTH: Jonathan Smoley
// DESC: Syntax Analysis for MyPL
//----------------------------------------------------------------------

#include "ast_parser.h"

using namespace std;


ASTParser::ASTParser(const Lexer& a_lexer)
  : lexer {a_lexer}
{}


void ASTParser::advance()
{
  curr_token = lexer.next_token();
}


void ASTParser::eat(TokenType t, const string& msg)
{
  if (!match(t))
    error(msg);
  advance();
}


bool ASTParser::match(TokenType t)
{
  return curr_token.type() == t;
}


bool ASTParser::match(initializer_list<TokenType> types)
{
  for (auto t : types)
    if (match(t))
      return true;
  return false;
}


void ASTParser::error(const string& msg)
{
  string s = msg + " found '" + curr_token.lexeme() + "' ";
  s += "at line " + to_string(curr_token.line()) + ", ";
  s += "column " + to_string(curr_token.column());
  throw MyPLException::ParserError(s);
}


bool ASTParser::bin_op()
{
  return match({TokenType::PLUS, TokenType::MINUS, TokenType::TIMES,
                TokenType::DIVIDE, TokenType::AND, TokenType::OR, TokenType::EQUAL,
                TokenType::LESS, TokenType::GREATER, TokenType::LESS_EQ,
                TokenType::GREATER_EQ, TokenType::NOT_EQUAL});
}


Program ASTParser::parse()
{
  Program p;
  advance();
  while (!match(TokenType::EOS))
  {
    if (match(TokenType::STRUCT))
      struct_def(p);
    else
      fun_def(p);
  }
  eat(TokenType::EOS, "expecting end-of-file");
  return p;
}


void ASTParser::struct_def(Program& p)
{
  StructDef s;
  eat(TokenType::STRUCT, "expecting 'struct'");

  s.struct_name = curr_token;
  eat(TokenType::ID, "expecting identifier");

  eat(TokenType::LBRACE, "expecting '{'");
  fields(s);
  eat(TokenType::RBRACE, "expecting '}'");

  p.struct_defs.push_back(s);
}


void ASTParser::fields(StructDef& s)
{
  if (base_type() || match({TokenType::ID, TokenType::ARRAY}))
  {
    VarDef v;
    data_type(v);
    v.var_name = curr_token;
    eat(TokenType::ID, "expecting identifier");
    s.fields.push_back(v);

    while (match(TokenType::COMMA))
    {
      eat(TokenType::COMMA, "expecting ','");
      VarDef v;
      data_type(v);
      v.var_name = curr_token;
      eat(TokenType::ID, "expecting identifier");
      s.fields.push_back(v);
    }
  }
}


void ASTParser::fun_def(Program& p)
{
  FunDef fun;

  VarDef v;
  data_type(v);
  fun.return_type = v.data_type;

  fun.fun_name = curr_token;
  eat(TokenType::ID, "expecting function name");

  eat(TokenType::LPAREN, "expecting '('");
  params(fun);
  eat(TokenType::RPAREN, "expecting ')'");
  eat(TokenType::LBRACE, "expecting '{'");

  vector<shared_ptr<Stmt>> s;
  stmt(s);
  fun.stmts = s;

  eat(TokenType::RBRACE, "expecting '}'");

  p.fun_defs.push_back(fun);
}


void ASTParser::params(FunDef& f)
{
  while (!match(TokenType::RPAREN))
  {
    VarDef v;
    data_type(v);

    v.var_name = curr_token;
    eat(TokenType::ID, "expecting identifier");

    f.params.push_back(v);

    if (match(TokenType::COMMA))
    {
      advance();
      if (match(TokenType::RPAREN))
        error("expecting another parameter");
    }
  }
}


void ASTParser::data_type(VarDef& v)
{
  if (match(TokenType::VOID_TYPE))
  {
    // v.data_type.type_name = curr_token.lexeme();
    v.data_type.type_names.push_back(curr_token.lexeme());
    eat(TokenType::VOID_TYPE, "expecting 'void'");
  }
  else if (base_type())
  {
    // v.data_type.type_name = curr_token.lexeme();
    v.data_type.type_names.push_back(curr_token.lexeme());
    eat(curr_token.type(), "expecting base type");
    if (!match(TokenType::ID))
      error("expecting identifier after base type");
  }
  else if (match(TokenType::ID))
  {
    // v.data_type.type_name = curr_token.lexeme();
    v.data_type.type_names.push_back(curr_token.lexeme());
    eat(TokenType::ID, "expecting identifier");
  }
  else if (match(TokenType::ARRAY))
  {
    v.data_type.is_array = true;
    eat(TokenType::ARRAY, "expecting 'array'");
    if (base_type() || match(TokenType::ID))
    {
      // v.data_type.type_name = curr_token.lexeme();
      v.data_type.type_names.push_back(curr_token.lexeme());
      eat(curr_token.type(), "expecting array type");
    }
  }
  else if (match(TokenType::DICT))
  {
    v.data_type.is_dict = true;
    eat(TokenType::DICT, "expecting 'dict'");
    // parse key type
    if (match({TokenType::ID, TokenType::STRING_TYPE, 
              TokenType::INT_TYPE, TokenType::CHAR_TYPE}))
    {
      // v.data_type.type_name = curr_token.lexeme();
      v.data_type.type_names.push_back(curr_token.lexeme());
      eat(curr_token.type(), "expecting type for key");
    }
    // parse value type
    if (base_type() || match({TokenType::ID, TokenType::ARRAY}))
    {
      // v.data_type.type_name = curr_token.lexeme();
      v.data_type.type_names.push_back(curr_token.lexeme());
      eat(curr_token.type(), "expecting type for value");
    }
  }
}


bool ASTParser::base_type()
{
  return match({TokenType::INT_TYPE, TokenType::DOUBLE_TYPE,
                TokenType::BOOL_TYPE, TokenType::CHAR_TYPE,
                TokenType::STRING_TYPE});
}


void ASTParser::stmt(std::vector<std::shared_ptr<Stmt>>& s)
{
  while (!match(TokenType::RBRACE))
  {
    if (match(TokenType::IF))
    {
      IfStmt iStmt;
      if_stmt(iStmt);
      s.push_back(make_shared<IfStmt>(iStmt));
    }
    else if (match(TokenType::FOR))
    {
      ForStmt fStmt;
      for_stmt(fStmt);
      s.push_back(make_shared<ForStmt>(fStmt));
    }
    else if (match(TokenType::WHILE))
    {
      WhileStmt wStmt;
      while_stmt(wStmt);
      s.push_back(make_shared<WhileStmt>(wStmt));
    }
    else if (match(TokenType::RETURN))
    {
      ReturnStmt rStmt;
      ret_stmt(rStmt);
      s.push_back(make_shared<ReturnStmt>(rStmt));
    }
    else if (match({TokenType::ARRAY, TokenType::DICT}) || base_type())
    {
      VarDeclStmt vDecl;
      data_type(vDecl.var_def);
      vdecl_stmt(vDecl);
      s.push_back(make_shared<VarDeclStmt>(vDecl));
    }
    else if (match(TokenType::ID))
    {
      Token id_token = curr_token;
      advance();
      if (match({TokenType::ASSIGN, TokenType::DOT}))
      {
        AssignStmt aStmt;
        VarRef v;
        v.var_name = id_token;
        aStmt.lvalue.push_back(v);
        assign_stmt(aStmt, id_token);
        s.push_back(make_shared<AssignStmt>(aStmt));
      }
      else if (match(TokenType::LBRACKET))
      {
        advance();
        AssignStmt aStmt;
        Expr e;
        expr(e);
        VarRef v;
        v.array_expr = e;
        v.var_name = id_token;
        aStmt.lvalue.push_back(v);
        eat(TokenType::RBRACKET, "expecting ']'");
        assign_stmt(aStmt, id_token);
        s.push_back(make_shared<AssignStmt>(aStmt));
      }
      else if (match(TokenType::LPAREN))
      {
        CallExpr cExpr;
        cExpr.fun_name = id_token;
        call_expr(cExpr);
        s.push_back(make_shared<CallExpr>(cExpr));
      }
      else
      {
        VarDeclStmt vDecl;
        vdecl_stmt(vDecl);
        // vDecl.var_def.data_type.type_name = id_token.lexeme();
        vDecl.var_def.data_type.type_names.push_back(id_token.lexeme());
        s.push_back(make_shared<VarDeclStmt>(vDecl));
      }
    }
    else
      error("expecting statement");
  }
}


void ASTParser::vdecl_stmt(VarDeclStmt& vDecl)
{
  if (!match(TokenType::ASSIGN))
  {
    vDecl.var_def.var_name = curr_token;
    eat(TokenType::ID, "expecting identifier");
  }
  eat(TokenType::ASSIGN, "expecting '='");
  expr(vDecl.expr);
}


void ASTParser::assign_stmt(AssignStmt& aStmt, Token t)
{
  while (!match(TokenType::ASSIGN))
  {
    VarRef v;
    lvalue(v, t);
    aStmt.lvalue.push_back(v);
  }
  eat(TokenType::ASSIGN, "expecting '='");
  expr(aStmt.expr);
}


void ASTParser::lvalue(VarRef& v, Token t)
{
  if (match(TokenType::ID))
  {
    v.var_name = curr_token;
    eat(TokenType::ID, "expecting identifier");
  }

  if (match(TokenType::DOT))
  {
    eat(TokenType::DOT, "expecting '.'");
    v.var_name = curr_token;
    eat(TokenType::ID, "expecting identifier");
  }

  if (match(TokenType::LBRACKET))
  {
    eat(TokenType::LBRACKET, "expecting '['");
    Expr e;
    expr(e);
    v.array_expr = make_optional<Expr>(e);
    eat(TokenType::RBRACKET, "expecting ']'");
  }
}


void ASTParser::if_stmt(IfStmt& iStmt)
{
  eat(TokenType::IF, "expecting 'if'");
  eat(TokenType::LPAREN, "expecting '('");

  expr(iStmt.if_part.condition);

  eat(TokenType::RPAREN, "expecting ')'");

  eat(TokenType::LBRACE, "expecting '{'");

  vector<shared_ptr<Stmt>> s;
  stmt(s);
  iStmt.if_part.stmts = s;

  eat(TokenType::RBRACE, "expecting '}'");

  if_stmt_t(iStmt);
}


void ASTParser::if_stmt_t(IfStmt& i)
{
  if (match(TokenType::ELSEIF))
  {
    BasicIf bi;

    eat(TokenType::ELSEIF, "expecting 'elseif'");

    expr(bi.condition);

    eat(TokenType::LBRACE, "expecting '{'");

    vector<shared_ptr<Stmt>> s;
    stmt(s);
    bi.stmts = s;

    eat(TokenType::RBRACE, "expecting '}'");
    i.else_ifs.push_back(bi);

    if_stmt_t(i);
  }
  else if (match(TokenType::ELSE))
  {
    eat(TokenType::ELSE, "expecting 'else'");
    eat(TokenType::LBRACE, "expecting '{'");

    vector<shared_ptr<Stmt>> s;
    stmt(s);
    i.else_stmts = s;

    eat(TokenType::RBRACE, "expecting '}'");
  }
}


void ASTParser::while_stmt(WhileStmt& wStmt)
{
  eat(TokenType::WHILE, "expecting 'while'");

  expr(wStmt.condition);

  eat(TokenType::LBRACE, "expecting '{'");

  vector<shared_ptr<Stmt>> s;
  stmt(s);
  wStmt.stmts = s;

  eat(TokenType::RBRACE, "expecting '}'");
}


void ASTParser::for_stmt(ForStmt& fStmt)
{
  eat(TokenType::FOR, "expecting 'for'");

  eat(TokenType::LPAREN, "expecting '('");

  VarDeclStmt vDecl;
  data_type(vDecl.var_def);
  vdecl_stmt(vDecl);
  fStmt.var_decl = vDecl;
  eat(TokenType::SEMICOLON, "expecting ';'");

  expr(fStmt.condition);

  eat(TokenType::SEMICOLON, "expecting ';'");

  AssignStmt aStmt;
  assign_stmt(aStmt, fStmt.var_decl.var_def.var_name);
  fStmt.assign_stmt = aStmt;

  eat(TokenType::RPAREN, "expecting ')'");

  eat(TokenType::LBRACE, "expecting '{'");

  vector<shared_ptr<Stmt>> s;
  stmt(s);
  fStmt.stmts = s;

  eat(TokenType::RBRACE, "expecting '}'");
}


void ASTParser::call_expr(CallExpr& cExpr)
{
  eat(TokenType::LPAREN, "expecting '('");
  if (base_rvalue() || match({TokenType::NULL_VAL,
                              TokenType::NEW, TokenType::ID}))
  {
    Expr e;
    expr(e);
    cExpr.args.push_back(e);
    while (match(TokenType::COMMA))
    {
      advance();
      Expr e;
      expr(e);
      cExpr.args.push_back(e);
    }
  }
  eat(TokenType::RPAREN, "expecting ')'");
}


void ASTParser::ret_stmt(ReturnStmt& rStmt)
{
  eat(TokenType::RETURN, "expecting 'return'");
  expr(rStmt.expr);
}


void ASTParser::expr(Expr& e)
{
  while (match(TokenType::NOT))
  {
    e.negated = true;
    advance();
  }

  if (match(TokenType::LPAREN))
  {
    advance();
    ComplexTerm cTerm;
    expr(cTerm.expr);
    e.first = make_shared<ComplexTerm>(cTerm);
    eat(TokenType::RPAREN, "expecting ')'");
  }
  else if (base_rvalue() || base_type() || match({TokenType::NULL_VAL, TokenType::NEW, TokenType::ID}))
  {
    SimpleTerm sTerm;
    rvalue(sTerm);
    e.first = make_shared<SimpleTerm>(sTerm);
  }
  else
    error("expecting expression start");

  if (bin_op())
  {
    e.op = curr_token;
    advance();
    Expr restExpr;
    expr(restExpr);
    e.rest = make_shared<Expr>(restExpr);
  }
}


void ASTParser::rvalue(SimpleTerm& sTerm)
{
  if (match(TokenType::NULL_VAL) || base_rvalue() || base_type())
  {
    SimpleRValue sRVal;
    sRVal.value = curr_token;
    advance();
    sTerm.rvalue = make_shared<SimpleRValue>(sRVal);
  }
  else if (match(TokenType::NEW))
  {
    advance();
    NewRValue newRVal;
    new_rvalue(newRVal);
    sTerm.rvalue = make_shared<NewRValue>(newRVal);
  }
  else if (match(TokenType::ID))
  {
    Token tmp = curr_token;
    advance();
    if (match(TokenType::LPAREN))
    {
      CallExpr cExpr;
      cExpr.fun_name = tmp;
      call_expr(cExpr);
      sTerm.rvalue = make_shared<CallExpr>(cExpr);
    }
    else
    {
      VarRef v;
      v.var_name = tmp;
      VarRValue vRVal;
      vRVal.path.push_back(v);
      var_rvalue(vRVal);
      sTerm.rvalue = make_shared<VarRValue>(vRVal);
    }
  }
  else
    error("expecting rvalue");
}


void ASTParser::new_rvalue(NewRValue& nRVal)
{
  if (match(TokenType::ID))
  {
    nRVal.type = curr_token;
    advance();
    if (match(TokenType::LBRACKET))
    {
      eat(TokenType::LBRACKET, "expecting '['");
      Expr e;
      expr(e);
      nRVal.array_expr = make_optional<Expr>(e);
      eat(TokenType::RBRACKET, "expecting ']'");
    }
  }
  else if (match(TokenType::DICT))
  {
    nRVal.type = curr_token;
    advance();
    eat(TokenType::LBRACE, "expecting '{'");
    std::vector<Expr> types;
    Expr e;
    expr(e);
    types.push_back(e);
    eat(TokenType::COMMA, "expecting ','");
    expr(e);
    types.push_back(e);
    nRVal.dict_expr = make_optional<std::vector<Expr>>(types);
    eat(TokenType::RBRACE, "expecting '}'");
  }
  else
  {
    if (base_type())
    {
      nRVal.type = curr_token;
      advance();
      eat(TokenType::LBRACKET, "expecting '['");
      Expr e;
      expr(e);
      nRVal.array_expr = make_optional<Expr>(e);
      eat(TokenType::RBRACKET, "expecting ']'");
    }
  }
}


bool ASTParser::base_rvalue()
{
  return match({TokenType::INT_VAL, TokenType::DOUBLE_VAL,
                TokenType::BOOL_VAL, TokenType::CHAR_VAL,
                TokenType::STRING_VAL});
}


void ASTParser::var_rvalue(VarRValue& vRVal)
{
  while (match({TokenType::DOT, TokenType::LBRACKET}))
  {
    if (match(TokenType::DOT))
    {
      advance();
      VarRef v;
      v.var_name = curr_token;
      vRVal.path.push_back(v);
      eat(TokenType::ID, "expecting identifier");
    }
    else if (match(TokenType::LBRACKET))
    {
      advance();
      Expr e;
      expr(e);
      vRVal.path.back().array_expr = make_optional<Expr>(e);
      eat(TokenType::RBRACKET, "expecting ']'");
    }
  }
}

