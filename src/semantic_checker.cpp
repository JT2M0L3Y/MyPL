//----------------------------------------------------------------------
// FILE: semantic_checker.cpp
// DATE: CPSC 326, Spring 2023
// AUTH: Jonathan Smoley
// DESC: Homework 5 - Semantic (type) Checker for MyPL
//----------------------------------------------------------------------

#include <unordered_set>
#include "mypl_exception.h"
#include "semantic_checker.h"
#include <iostream>

using namespace std;

// hash table of names of the base data types and built-in functions
const unordered_set<string> BASE_TYPES{"int", "double", "char", "string", "bool"};
const unordered_set<string> BUILT_INS{"print", "input", "to_string", "to_int",
                                      "to_double", "length", "get", "concat",
                                      "keys", "values"};

// helper functions

optional<VarDef> SemanticChecker::get_field(const StructDef &struct_def,
                                            const string &field_name)
{
  for (const VarDef &var_def : struct_def.fields)
    if (var_def.var_name.lexeme() == field_name)
      return var_def;
  return nullopt;
}

void SemanticChecker::error(const string &msg, const Token &token)
{
  string s = msg;
  s += " near line " + to_string(token.line()) + ", ";
  s += "column " + to_string(token.column());
  throw MyPLException::StaticError(s);
}

void SemanticChecker::error(const string &msg)
{
  throw MyPLException::StaticError(msg);
}

// visitor functions

void SemanticChecker::visit(Program &p)
{
  // record each struct def
  for (StructDef &d : p.struct_defs)
  {
    string name = d.struct_name.lexeme();
    if (struct_defs.contains(name))
      error("multiple definitions of '" + name + "'", d.struct_name);
    struct_defs[name] = d;
  }
  // record each function def (need a main function)
  bool found_main = false;
  for (FunDef &f : p.fun_defs)
  {
    string name = f.fun_name.lexeme();
    if (BUILT_INS.contains(name))
      error("redefining built-in function '" + name + "'", f.fun_name);
    if (fun_defs.contains(name))
      error("multiple definitions of '" + name + "'", f.fun_name);
    if (name == "main")
    {
      if (f.return_type.type_names[0] != "void")
        error("main function must have void type", f.fun_name);
      if (f.params.size() != 0)
        error("main function cannot have parameters", f.params[0].var_name);
      found_main = true;
    }
    fun_defs[name] = f;
  }
  if (!found_main)
    error("program missing main function");
  // check each struct
  for (StructDef &d : p.struct_defs)
    d.accept(*this);
  // check each function
  for (FunDef &d : p.fun_defs)
    d.accept(*this);
}

void SemanticChecker::visit(SimpleRValue &v)
{
  if (v.value.type() == TokenType::INT_VAL)
    curr_type = DataType{false, false, {"int"}};
  else if (v.value.type() == TokenType::DOUBLE_VAL)
    curr_type = DataType{false, false, {"double"}};
  else if (v.value.type() == TokenType::CHAR_VAL)
    curr_type = DataType{false, false, {"char"}};
  else if (v.value.type() == TokenType::STRING_VAL)
    curr_type = DataType{false, false, {"string"}};
  else if (v.value.type() == TokenType::BOOL_VAL)
    curr_type = DataType{false, false, {"bool"}};
  else if (v.value.type() == TokenType::NULL_VAL)
    curr_type = DataType{false, false, {"void"}};
}

void SemanticChecker::visit(FunDef &f)
{
  // create a new scope environment
  symbol_table.push_environment();

  // check if return type is valid
  if (!(struct_defs.contains(f.return_type.type_names[0]) || f.return_type.type_names[0] == "void" || 
        BASE_TYPES.contains(f.return_type.type_names[0])))
    error("function return type cannot be '" + f.return_type.type_names[0] + "'", f.fun_name);

  // add function return type to environment
  symbol_table.add("return", f.return_type);

  // check params, add to environment
  for (VarDef &v : f.params)
  {
    // check if param already defined
    if (symbol_table.name_exists_in_curr_env(v.var_name.lexeme()))
      error("multiple definitions of '" + v.var_name.lexeme() + "'", v.var_name);
    // check if invalid type or struct
    if (!(struct_defs.contains(v.data_type.type_names[0]) || 
          BASE_TYPES.contains(v.data_type.type_names[0])))
      error("function parameter cannot have type '" + v.data_type.type_names[0] + "'", v.var_name);
    // add param to environment
    symbol_table.add(v.var_name.lexeme(), v.data_type);
  }

  // check statements
  for (auto &s : f.stmts)
    s->accept(*this);

  // destroy the scope environment
  symbol_table.pop_environment();
}

void SemanticChecker::visit(StructDef &s)
{
  // create a new environment
  symbol_table.push_environment();

  // check all field types are not void & record them in environment
  for (VarDef &v : s.fields)
  {
    // check for existing field defined
    if (symbol_table.name_exists_in_curr_env(v.var_name.lexeme()))
      error("multiple definitions of '" + v.var_name.lexeme() + "'", v.var_name);
    // check for non self or base type field
    if (!(struct_defs.contains(v.data_type.type_names[0]) || 
          BASE_TYPES.contains(v.data_type.type_names[0])))
      error("struct field cannot have type '" + v.data_type.type_names[0] + "'", v.var_name);
    // add field to environment
    symbol_table.add(v.var_name.lexeme(), v.data_type);
  }

  // destroy environment
  symbol_table.pop_environment();
}

void SemanticChecker::visit(ReturnStmt &s)
{
  s.expr.accept(*this);

  if (symbol_table.get("return")->type_names[0] != curr_type.type_names[0] && 
      curr_type.type_names[0] != "void")
    error("type mismatch in return statement", s.expr.first_token());
}

void SemanticChecker::visit(WhileStmt &s)
{
  // check condition
  s.condition.accept(*this);
  if (curr_type.type_names[0] != "bool")
    error("while condition must be of type bool", s.condition.first->first_token());

  // create a new scope environment
  symbol_table.push_environment();

  // visit statements
  for (auto &stmt : s.stmts)
    stmt->accept(*this);

  // destroy the scope environment
  symbol_table.pop_environment();
}

void SemanticChecker::visit(ForStmt &s)
{
  // create a new scope environment
  symbol_table.push_environment();

  // check var decl
  if (s.var_decl.var_def.data_type.type_names[0] != "int")
    error("for iterator must be integer type", s.var_decl.var_def.first_token());
  s.var_decl.accept(*this);

  // add var to environment
  symbol_table.add(s.var_decl.var_def.var_name.lexeme(), s.var_decl.var_def.data_type);

  // check condition
  s.condition.accept(*this);
  if (curr_type.type_names[0] != "bool")
    error("condition must be of bool type", s.condition.first_token());

  // check assignment
  s.assign_stmt.accept(*this);

  // visit statements
  for (auto &stmt : s.stmts)
    stmt->accept(*this);

  // destroy the scope environment
  symbol_table.pop_environment();
}

void SemanticChecker::visit(IfStmt &s)
{
  // if block
  BasicIf base_if = s.if_part;
  // check condition type
  base_if.condition.accept(*this);
  if (curr_type.type_names[0] != "bool" || curr_type.is_array)
    error("if condition must be of type bool", base_if.condition.first->first_token());

  symbol_table.push_environment();
  // parse block statements
  for (auto &stmt : base_if.stmts)
    stmt->accept(*this);
  symbol_table.pop_environment();

  // else if block(s)
  for (BasicIf &else_if : s.else_ifs)
  {
    else_if.condition.accept(*this);
    if (curr_type.type_names[0] != "bool" || curr_type.is_array)
      error("else if condition must be of type bool", else_if.condition.first->first_token());

    symbol_table.push_environment();
    for (auto &stmt : else_if.stmts)
      stmt->accept(*this);
    symbol_table.pop_environment();
  }

  // else block
  symbol_table.push_environment();
  if (s.else_stmts.size() > 0)
  {
    for (auto &stmt : s.else_stmts)
      stmt->accept(*this);
  }
  symbol_table.pop_environment();
}

void SemanticChecker::visit(VarDeclStmt &s)
{
  // check that defined var name has not been used
  if (symbol_table.name_exists_in_curr_env(s.var_def.var_name.lexeme()))
    error("multiple definitions of '" + s.var_def.var_name.lexeme() + "'", s.var_def.var_name);

  // check that defined var has valid type
  if (!BASE_TYPES.contains(s.var_def.data_type.type_names[0]) && 
      !struct_defs.contains(s.var_def.data_type.type_names[0]))
    error("undefined type in var decl", s.var_def.first_token());

  // store lhs type
  DataType lhs_type = s.var_def.data_type;

  // update curr_type to rhs expr type
  s.expr.accept(*this);
  DataType rhs_type = curr_type;

  // check var defined type matches rhs expr type
  for (int i = 0; i < lhs_type.type_names.size(); i++)
  {
    if ((lhs_type.type_names[i] != rhs_type.type_names[i]) && rhs_type.type_names[i] != "void")
      error("type mismatch in var decl between '" + lhs_type.type_names[i] + "' and '" +
                rhs_type.type_names[i] + "'", s.expr.first_token());
  }

  // add var to environment
  symbol_table.add(s.var_def.var_name.lexeme(), s.var_def.data_type);
}

void SemanticChecker::visit(AssignStmt &s)
{
  // get first var
  string vname = s.lvalue[0].var_name.lexeme();
  // check if var exists
  if (!symbol_table.name_exists(vname))
    error("variable '" + vname + "' not defined", s.lvalue[0].var_name);
  // get left hand side type
  DataType lhs_type = *symbol_table.get(vname);

  // complex paths
  for (int i = 1; i < s.lvalue.size(); i++)
  {
    // check for struct
    if (!get_field(struct_defs[lhs_type.type_names[0]], s.lvalue[i].var_name.lexeme()))
      error("struct '" + lhs_type.type_names[0] + "' does not have a field '" + 
            vname + "'", s.lvalue[i].var_name);

    lhs_type = get_field(struct_defs[vname], s.lvalue[i].var_name.lexeme())->data_type;
  }

  // track rhs type
  s.expr.accept(*this);
  DataType rhs_type = curr_type;

  // check if types match
  if (lhs_type.type_names[1] != rhs_type.type_names[0] && rhs_type.type_names[0] != "void")
    error("types '" + lhs_type.type_names[1] + "' and '" + rhs_type.type_names[0] +
              "' do not match", s.expr.first_token());
}

void SemanticChecker::visit(CallExpr &e)
{
  string fname = e.fun_name.lexeme();
  // check if built in function
  if (BUILT_INS.contains(fname))
  {
    if (fname == "print")
    {
      // check if correct arg count
      if (e.args.size() != 1)
        error("print expects 1 argument", e.fun_name);
      e.args[0].accept(*this);
      // check if valid arg type is passed
      if (!BASE_TYPES.contains(curr_type.type_names[0]) && !curr_type.is_array && !curr_type.is_dict)
        error("cannot print non-primitive type", e.args[0].first->first_token());
      curr_type = DataType{false, false, {"void"}};
    }
    else if (fname == "input")
    {
      // check if correct arg count
      if (e.args.size() != 0)
        error("input does not expect arguments", e.fun_name);
      curr_type = DataType{false, false, {"string"}};
    }
    else if (fname == "to_string")
    {
      // check if correct arg count
      if (e.args.size() != 1)
        error("to_string expects 1 argument", e.fun_name);
      e.args[0].accept(*this);
      if (curr_type.type_names[0] != "int" && curr_type.type_names[0] != "double" && 
          curr_type.type_names[0] != "char")
        error("to_string expects int/double/char type", e.args[0].first_token());
      curr_type = DataType{false, false, {"string"}};
    }
    else if (fname == "to_int")
    {
      // check if correct arg count
      if (e.args.size() != 1)
        error("to_int expects 1 argument", e.fun_name);
      e.args[0].accept(*this);
      if (curr_type.type_names[0] == "int" || curr_type.type_names[0] == "bool")
        error("to_int expects non-int types", e.args[0].first_token());
      curr_type = DataType{false, false, {"int"}};
    }
    else if (fname == "to_double")
    {
      // check if correct arg count
      if (e.args.size() != 1)
        error("to_double expects 1 argument", e.fun_name);
      e.args[0].accept(*this);
      if (curr_type.type_names[0] != "string" && curr_type.type_names[0] != "int")
        error("to_double expects string or int arg type", e.args[0].first_token());
      curr_type = DataType{false, false, {"double"}};
    }
    else if (fname == "length")
    {
      // check if correct arg count
      if (e.args.size() != 1)
        error("length expects 1 argument", e.fun_name);
      e.args[0].accept(*this);
      if (curr_type.type_names[0] != "string" && !curr_type.is_array)
        error("length expects string or array", e.args[0].first_token());
      if (curr_type.is_array) 
      {
        curr_type = DataType{true, false, {"int"}};
        e.fun_name = Token(e.fun_name.type(), "length@array", e.fun_name.line(), e.fun_name.column());
      }
      else
        curr_type = DataType{false, false, {"int"}};
    }
    else if (fname == "get")
    {
      // check if correct arg count
      if (e.args.size() != 2)
        error("get expects 2 argument", e.fun_name);
      e.args[0].accept(*this);
      if (curr_type.type_names[0] != "int")
        error("get expects int", e.fun_name);
      e.args[1].accept(*this);
      if (curr_type.type_names[0] != "string")
        error("get expects string", e.fun_name);
      curr_type = DataType{false, false, {"char"}};
    }
    else if (fname == "concat")
    {
      // check if correct arg count
      if (e.args.size() != 2)
        error("concat expects 2 argument", e.fun_name);
      e.args[0].accept(*this);
      if (curr_type.type_names[0] != "string")
        error("concat expects string", e.fun_name);
      e.args[1].accept(*this);
      if (curr_type.type_names[0] != "string")
        error("concat expects string", e.fun_name);
      curr_type = DataType{false, false, {"string"}};
    }
    else if (fname == "keys")
    {
      // check arg count
      if (e.args.size() != 1)
        error("keys expects 1 argument", e.fun_name);
      e.args[0].accept(*this);
      if (!curr_type.is_dict)
        error("keys only operates on dictionaries", e.fun_name);
      curr_type = DataType{true, false, {curr_type.type_names[0]}};
    }
    else if (fname == "values")
    {
      // check arg count
      if (e.args.size() != 1)
        error("values expects 1 argument", e.fun_name);
      e.args[0].accept(*this);
      if (!curr_type.is_dict)
        error("values only operates on dictionaries", e.fun_name);
      curr_type = DataType{true, false, {curr_type.type_names[1]}};
    }
  }
  else
  {
    // check user function exists
    if (!fun_defs.contains(fname))
      error("function '" + fname + "' was not defined", e.fun_name);
    // check correct arg count
    if (e.args.size() != fun_defs[fname].params.size())
      error("function '" + fname + "' expects " + to_string(fun_defs[fname].params.size()) + 
            " arguments", e.fun_name);
    // check arg types
    for (int i = 0; i < e.args.size(); i++)
    {
      e.args[i].accept(*this);
      // check if types match
      if (curr_type.type_names[0] != fun_defs[fname].params[i].data_type.type_names[0] && 
          curr_type.type_names[0] != "void")
        error("function '" + fname + "' expects argument " + to_string(i + 1) + 
              " to be of type " + fun_defs[fname].params[i].data_type.type_names[0], e.fun_name);
    }
    // set return type
    curr_type = fun_defs[fname].return_type;
  }
}

void SemanticChecker::visit(Expr &e)
{
  DataType first, rest;
  Token operation;
  // log first expr term type
  e.first->accept(*this);
  first = curr_type;
  // type check if op is not empty
  if (e.op.has_value())
  {
    // log operand type
    operation = *e.op;
    // get rest type
    e.rest->accept(*this);
    rest = curr_type;
    // check if valid and/or expression
    if ((operation.type() == TokenType::AND) || (operation.type() == TokenType::OR))
    {
      if (first.type_names[0] != rest.type_names[0] && (first.type_names[0] != "void" || rest.type_names[0] != "void") &&
          (first.type_names[0] == "void" && struct_defs.contains(rest.type_names[0])) &&
          (struct_defs.contains(first.type_names[0]) && rest.type_names[0] == "void"))
        error("using and/or ops on non-bool types '" + first.type_names[0] 
              + "' and '" + rest.type_names[0] + "'", e.first->first_token());
      curr_type = DataType{false, false, {"bool"}};
    }
    // check if valid arithmetic expression
    else if ((operation.type() == TokenType::PLUS) || (operation.type() == TokenType::MINUS) || 
             (operation.type() == TokenType::TIMES) || (operation.type() == TokenType::DIVIDE))
    {
      // check if both sides are ints or both sides are doubles
      if (!(first.type_names[0] == "int" && rest.type_names[0] == "int") &&
          !(first.type_names[0] == "double" && rest.type_names[0] == "double"))
        error("using arithmetic ops on non-int/double types '" + first.type_names[0] + 
              "' and '" + rest.type_names[0] + "'", e.first->first_token());
      curr_type = first;
    }
    // dictionary declaration rvalue
    else if (operation.type() == TokenType::COMMA)
    {
      if (!BASE_TYPES.contains(first.type_names[0]) || !BASE_TYPES.contains(rest.type_names[0]))
        error("invalid types used in dictionary declaration", e.first->first_token());

      curr_type = DataType{false, true, {first.type_names[0], rest.type_names[0]}};
    }
    // logical operators
    else
    {
      // equals/not equals
      if ((operation.type() == TokenType::EQUAL) || (operation.type() == TokenType::NOT_EQUAL))
      {
        // check if valid types used
        if ((first.type_names[0] != "void" || rest.type_names[0] != "void") &&
            (first.type_names[0] == "void" && struct_defs.contains(rest.type_names[0])) &&
            (struct_defs.contains(first.type_names[0]) && rest.type_names[0] == "void"))
          error("using equals/not-equals ops on mismatched types '" + first.type_names[0] + 
                "' and '" + rest.type_names[0] + "'", e.first->first_token());
      }
      // LT, LE, GT, GE
      else
      {
        // check if used with non-comparable type
        if (first.type_names[0] != rest.type_names[0] || first.type_names[0] == "bool" || rest.type_names[0] == "bool"
            || first.is_array || rest.is_array || first.is_dict || rest.is_dict)
          error("using relational ops on non-comparable types '" + first.type_names[0] +
                    "' and '" + rest.type_names[0] + "'", e.first->first_token());
      }
      curr_type = DataType{false, false, {"bool"}};
    }
  }
  // if lhs, but not rhs then type is lhs
  else if (e.first != nullptr && e.rest == nullptr)
    curr_type = first;
  // otherwise, type is void
  else
    curr_type = DataType{false, false, {"void"}};
  // check type in negation if present
  if (e.negated && curr_type.type_names[0] != "bool")
    error("negating non-bool type '" + curr_type.type_names[0] + "'", e.first->first_token());
}

void SemanticChecker::visit(SimpleTerm &t)
{
  t.rvalue->accept(*this);
}

void SemanticChecker::visit(ComplexTerm &t)
{
  t.expr.accept(*this);
}

void SemanticChecker::visit(NewRValue &v)
{
  if (v.array_expr.has_value())
  {
    v.array_expr->accept(*this);
    // check if name is valid
    if (curr_type.type_names[0] != "int")
      error("type '" + v.type.lexeme() + "' not defined", v.type);
    curr_type = DataType{true, false, {v.type.lexeme()}};
  }
  else if (v.dict_expr.has_value())
  {
    std::vector<std::string> types;
    v.dict_expr->first->accept(*this);
    DataType type = curr_type;

    if (type.type_names[0] != "string" && type.type_names[0] != "int")
      error("key type '" + type.type_names[0] + "' not defined", v.type);
    types.push_back(type.type_names[0]);

    if (!BASE_TYPES.contains(type.type_names[1]) && !struct_defs.contains(type.type_names[1]))
      error("value type '" + type.type_names[1] + "' not defined", v.type);
    types.push_back(type.type_names[1]);

    curr_type = DataType{false, true, types};
  }
  else
    curr_type = DataType{false, false, {v.type.lexeme()}};
}

void SemanticChecker::visit(VarRValue &v)
{
  if (!symbol_table.name_exists(v.path[0].var_name.lexeme()))
    error("variable '" + v.path[0].var_name.lexeme() + "' not defined", v.path[0].var_name);
  // check if defined
  if (v.path[0].array_expr.has_value()) 
  {
    v.path[0].array_expr->accept(*this);
    if (curr_type.type_names[0] != "int")
      error("type '" + v.path[0].var_name.lexeme() + "' not defined", v.path[0].var_name);
    curr_type = symbol_table.get(v.path[0].var_name.lexeme()).value();
  }
  else if (v.path[0].dict_expr.has_value())
  {
    v.path[0].dict_expr->accept(*this);
    if (curr_type.type_names[0] != "string" && curr_type.type_names[0] != "int")
      error("type '" + v.path[0].var_name.lexeme() + "' not defined", v.path[0].var_name);
    else if (!BASE_TYPES.contains(curr_type.type_names[1]))
      error("type '" + v.path[0].var_name.lexeme() + "' not defined", v.path[1].var_name);
    curr_type = symbol_table.get(v.path[0].var_name.lexeme()).value();
  }
  else 
  {
    // get type
    curr_type = *symbol_table.get(v.path[0].var_name.lexeme());
  }
  // complex paths
  if (v.path.size() > 1) 
  {
    DataType var_type = symbol_table.get(v.path[0].var_name.lexeme()).value();
    StructDef def = struct_defs[var_type.type_names[0]];
    for (int i = 1; i < v.path.size(); i++)
    {
      // check if struct
      if (!get_field(def, v.path[i].var_name.lexeme()))
        error("struct '" + v.path[0].var_name.lexeme() + "' does not have a field '" +
              v.path[i].var_name.lexeme() + "'", v.path[i].var_name);
      curr_type = get_field(def, v.path[i].var_name.lexeme())->data_type;
      def = struct_defs[curr_type.type_names[0]];
    }
  }
}
