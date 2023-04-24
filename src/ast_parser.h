//----------------------------------------------------------------------
// FILE: ast_parser.h
// DATE: CPSC 326, Spring 2023
// AUTH: Jonathan Smoley
// DESC: Syntax Analysis for MyPL
//----------------------------------------------------------------------

#ifndef AST_PARSER_H
#define AST_PARSER_H

#include "mypl_exception.h"
#include "lexer.h"
#include "ast.h"


class ASTParser
{
public:

  // crate a new recursive descent parer
  ASTParser(const Lexer& lexer);

  // run the parser
  Program parse();
  
private:
  
  Lexer lexer;
  Token curr_token;
  
  // helper functions
  void advance();
  void eat(TokenType t, const std::string& msg);
  bool match(TokenType t);
  bool match(std::initializer_list<TokenType> types);
  void error(const std::string& msg);
  bool bin_op();

  // recursive descent functions
  void struct_def(Program& p);
  void fields(StructDef& s);
  void fun_def(Program& p);
  void params(FunDef& f);

  void data_type(VarDef& v);
  bool base_type();

  void stmt(std::vector<std::shared_ptr<Stmt>>& stmts);
  void vdecl_stmt(VarDeclStmt& vDecl);
  void assign_stmt(AssignStmt &aStmt, Token t);
  void if_stmt(IfStmt &iStmt);
  void if_stmt_t(IfStmt& iStmt);
  void while_stmt(WhileStmt& wStmt);
  void for_stmt(ForStmt& fStmt);
  void call_expr(CallExpr& cExpr);
  void ret_stmt(ReturnStmt& rStmt);

  void lvalue(VarRef& v, Token t);
  void rvalue(SimpleTerm &sTerm);
  void new_rvalue(NewRValue &nRVal);
  bool base_rvalue();
  void var_rvalue(VarRValue &vRVal);

  void expr(Expr& e);
};


#endif
