//----------------------------------------------------------------------
// FILE: print_visitor.cpp
// DATE: CPSC 326, Spring 2023
// AUTH: Jonathan Smoley
// DESC: Visitor Pattern Pretty Printer for MyPL AST
//----------------------------------------------------------------------

#include "print_visitor.h"

using namespace std;


PrintVisitor::PrintVisitor(ostream& output)
  : out(output)
{
}


void PrintVisitor::inc_indent()
{
  indent += INDENT_AMT;
}


void PrintVisitor::dec_indent()
{
  indent -= INDENT_AMT;
}


void PrintVisitor::print_indent()
{
  out << string(indent, ' ');
}


void PrintVisitor::visit(Program& p)
{
  for (auto struct_def : p.struct_defs)
    struct_def.accept(*this);
  for (auto fun_def : p.fun_defs)
    fun_def.accept(*this);
}

void PrintVisitor::visit(FunDef &f)
{
  out << endl;
  // return type
  if (f.return_type.is_array)
  {
    out << "array ";
    out << "[] ";
  }
  out << f.return_type.type_name << " ";

  // function name
  out << f.fun_name.lexeme() << "(";

  // parameters
  for (int i = 0; i < f.params.size(); i++)
  {
    if (f.params[i].data_type.is_array)
    {
      out << "array ";
      out << f.params[i].data_type.type_name;
      out << "[] ";
    }
    else
      out << f.params[i].data_type.type_name << " ";

    // check for end of list
    if (i != f.params.size() - 1)
      out << f.params[i].var_name.lexeme() << ", ";
    else
      out << f.params[i].var_name.lexeme();
  }
  out << ") ";

  // function body
  out << "{\n";
  inc_indent();
  for (auto stmt : f.stmts)
  {
    print_indent();
    stmt->accept(*this);
    out << endl;
  }
  out << "}\n";

  dec_indent();
  print_indent();
}

void PrintVisitor::visit(StructDef &s)
{
  // struct name
  out << "\nstruct " << s.struct_name.lexeme() << " {\n";

  // struct members
  inc_indent();
  for (int i = 0; i < s.fields.size(); i++)
  {
    print_indent();
    if (s.fields[i].data_type.is_array)
    {
      out << "array ";
      out << s.fields[i].data_type.type_name;
      out << "[] ";
    }
    else
      out << s.fields[i].data_type.type_name << " ";

    if (i == s.fields.size() - 1)
      out << s.fields[i].var_name.lexeme() << "\n";
    else
      out << s.fields[i].var_name.lexeme() << ",\n";
  }
  out << "}\n";

  dec_indent();
  print_indent();
}

void PrintVisitor::visit(ReturnStmt &s)
{
  out << "return ";
  s.expr.accept(*this);
}

void PrintVisitor::visit(WhileStmt &s)
{
  out << "while ";
  s.condition.accept(*this);
  out << " {\n";
  inc_indent();
  for (auto stmt : s.stmts)
  {
    print_indent();
    stmt->accept(*this);
    out << endl;
  }

  dec_indent();
  print_indent();

  out << "}";
}

void PrintVisitor::visit(ForStmt &s)
{
  int start_indent = indent;
  out << "for (";
  s.var_decl.accept(*this);
  out << "; ";
  s.condition.accept(*this);
  out << "; ";
  s.assign_stmt.accept(*this);
  out << ") {\n";
  inc_indent();
  for (auto stmt : s.stmts)
  {
    print_indent();
    stmt->accept(*this);
    out << endl;
  }

  dec_indent();
  print_indent();

  out << "}";
}

void PrintVisitor::visit(IfStmt &s)
{
  out << "if (";
  s.if_part.condition.accept(*this);
  out << ") {\n";
  inc_indent();

  for (auto stmt : s.if_part.stmts)
  {
    print_indent();
    stmt->accept(*this);
    out << endl;
  }

  if (s.else_ifs.size() != 0)
  {
    for (BasicIf bi : s.else_ifs)
    {
      dec_indent();
      print_indent();
      out << "}\n";
      print_indent();
      out << "elseif ";
      bi.condition.accept(*this);
      out << " {\n";
      inc_indent();

      for (auto stmt : bi.stmts)
      {
        print_indent();
        stmt->accept(*this);
        out << endl;
      }
    }
  }

  if (s.else_stmts.size() != 0)
  {
    dec_indent();
    print_indent();
    out << "}\n";
    print_indent();
    out << "else {\n";
    inc_indent();

    for (auto stmt : s.else_stmts)
    {
      print_indent();
      stmt->accept(*this);
      out << endl;
    }
  }

  dec_indent();
  print_indent();

  out << "}";
}

void PrintVisitor::visit(VarDeclStmt &s)
{
  if (s.var_def.data_type.is_array)
  {
    out << "array ";
    out << s.var_def.data_type.type_name;
    out << "[] ";
  }
  else
    out << s.var_def.data_type.type_name << " ";
  
  out << s.var_def.var_name.lexeme() << " = ";
  s.expr.accept(*this);
}

void PrintVisitor::visit(AssignStmt &s)
{
  if (s.lvalue.size() == 1)
    out << s.lvalue.front().var_name.lexeme();
  else
  {
    for (int i = 0; i < s.lvalue.size(); i++)
    {
      out << s.lvalue[i].var_name.lexeme();
      if (i != s.lvalue.size() - 1)
        out << ".";
    }
  }
  out << " = ";
  s.expr.accept(*this);
}

void PrintVisitor::visit(CallExpr &e)
{
  out << e.fun_name.lexeme() << "(";
  if (e.args.size() == 1)
    e.args.front().accept(*this);
  else if (e.args.size() > 1)
  {
    for (int i = 0; i < e.args.size(); i++)
    {
      e.args[i].accept(*this);
      if (i != e.args.size() - 1)
        out << ", ";
    }
  }
  out << ")";
}

void PrintVisitor::visit(Expr &e)
{
  if (e.op.has_value())
  {
    if (e.negated)
    {
      out << "not (";
      e.first->accept(*this);
      out << " " << e.op->lexeme() << " ";
      e.rest->accept(*this);
      out << ")";
    }
    else
    {
      e.first->accept(*this);
      out << " " << e.op->lexeme() << " ";
      e.rest->accept(*this);
    }
  }
  else
  {
    if (e.negated)
    {
      out << "not (";
      e.first->accept(*this);
      out << ")";
    }
    else
      e.first->accept(*this);
  }
}

void PrintVisitor::visit(SimpleTerm &t)
{
  t.rvalue->accept(*this);
}

void PrintVisitor::visit(ComplexTerm &t)
{
  out << "(";
  t.expr.accept(*this);
  out << ")";
}

void PrintVisitor::visit(SimpleRValue &v)
{
  if (v.value.type() == TokenType::STRING_VAL)
    out << "\"" << v.value.lexeme() << "\"";
  else if (v.value.type() == TokenType::CHAR_VAL)
    out << "'" << v.value.lexeme() << "'";
  else
    out << v.value.lexeme();
}

void PrintVisitor::visit(NewRValue &v)
{
  out << "new " << v.type.lexeme();
}

void PrintVisitor::visit(VarRValue &v)
{
  if (v.path.size() == 1)
    out << v.path.front().var_name.lexeme();
  else
  {
    for (int i = 0; i < v.path.size(); i++)
    {
      if (i == 0)
        out << v.path[i].var_name.lexeme();
      else
        out << "." << v.path[i].var_name.lexeme();
    }
  }
}