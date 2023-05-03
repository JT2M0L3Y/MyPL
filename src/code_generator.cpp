//----------------------------------------------------------------------
// FILE: code_generator.cpp
// DATE: CPSC 326, Spring 2023
// AUTH: Jonathan Smoley
// DESC: Code Generator
//----------------------------------------------------------------------

#include <iostream>             // for debugging
#include "code_generator.h"

using namespace std;


// helper function to replace all occurrences of old string with new
void replace_all(string& s, const string& old_str, const string& new_str)
{
  while (s.find(old_str) != string::npos)
    s.replace(s.find(old_str), old_str.size(), new_str);
}


CodeGenerator::CodeGenerator(VM& vm)
  : vm(vm)
{
}


void CodeGenerator::visit(Program& p)
{
  for (auto& struct_def : p.struct_defs)
    struct_def.accept(*this);
  for (auto& fun_def : p.fun_defs)
    fun_def.accept(*this);
}


void CodeGenerator::visit(FunDef& f)
{
  // create new frame (as curr_frame)
  curr_frame = VMFrameInfo();
  curr_frame.arg_count = f.params.size();
  curr_frame.function_name = f.fun_name.lexeme();
  // push a new var_table environment
  var_table.push_environment();
  // store each arg provided on operand stack (from CALL)
  if (!f.params.empty()) {
    int index = 0;
    for (int i = 0; i < f.params.size(); i++) {
      var_table.add(f.params[i].var_name.lexeme());
      index = var_table.get(f.params[i].var_name.lexeme());
      curr_frame.instructions.push_back(VMInstr::STORE(index));
    }
  }
  // visit each stmt
  for (auto &s : f.stmts)
    s->accept(*this);
  // add a return if last stmt was not a return
  int instr_count = curr_frame.instructions.size();
  if (f.stmts.size() == 0 || (f.return_type.type_name == "void" && 
      curr_frame.instructions[instr_count - 1].opcode() != OpCode::RET)) 
  {
    curr_frame.instructions.push_back(VMInstr::PUSH(nullptr));
    curr_frame.instructions.push_back(VMInstr::RET());
  }
  // pop var environment
  var_table.pop_environment();
  // add the frame to the VM
  vm.add(curr_frame);
}


void CodeGenerator::visit(StructDef& s)
{
  // store struct def for later
  struct_defs[s.struct_name.lexeme()] = s;
}


void CodeGenerator::visit(ReturnStmt& s)
{
  s.expr.accept(*this);
  curr_frame.instructions.push_back(VMInstr::RET());
}


void CodeGenerator::visit(WhileStmt& s)
{
  int loop_index = curr_frame.instructions.size();
  // call the while condition visitor
  s.condition.accept(*this);
  // create & add a JMPF instr w/ tmp operand -1 (jmp to end)
  curr_frame.instructions.push_back(VMInstr::JMPF(-1));
  int jmpf_index = curr_frame.instructions.size() - 1;
  var_table.push_environment();
  for (auto& s : s.stmts)
    s->accept(*this);
  var_table.pop_environment();
  // add JMP instr (to jmp to start index)
  curr_frame.instructions.push_back(VMInstr::JMP(loop_index));
  curr_frame.instructions[jmpf_index] = VMInstr::JMPF(curr_frame.instructions.size());
}


void CodeGenerator::visit(ForStmt& s)
{
  // push environment for var decl (gen var decl code)
  var_table.push_environment();
  s.var_decl.accept(*this);
  // rest similar as while (within another push & popped environment)
  int loop_index = curr_frame.instructions.size();
  s.condition.accept(*this);
  curr_frame.instructions.push_back(VMInstr::JMPF(-1));
  int jmpf_index = curr_frame.instructions.size() - 1;
  var_table.push_environment();
  for (auto &s : s.stmts)
    s->accept(*this);
  var_table.pop_environment();
  // add assign stmt code before final JMP
  s.assign_stmt.accept(*this);
  // pop environment for var decl
  var_table.pop_environment();
  // add JMP, NOP, update JMPF
  curr_frame.instructions.push_back(VMInstr::JMP(loop_index));
  curr_frame.instructions[jmpf_index] = VMInstr::JMPF(curr_frame.instructions.size());
}


void CodeGenerator::visit(IfStmt& s)
{
  vector<int> jumps;
  // if part
  s.if_part.condition.accept(*this);
  var_table.push_environment();
  curr_frame.instructions.push_back(VMInstr::JMPF(-1));
  int jmpf_index = curr_frame.instructions.size() - 1;
  for (auto& is : s.if_part.stmts)
    is->accept(*this);
  var_table.pop_environment();
  // jump to end
  if (!s.else_ifs.empty() || !s.else_stmts.empty()) {
    curr_frame.instructions.push_back(VMInstr::JMP(-1));
    jumps.push_back(curr_frame.instructions.size() - 1);
  }
  curr_frame.instructions[jmpf_index] = VMInstr::JMPF(curr_frame.instructions.size());
  // else if part
  if (!s.else_ifs.empty()) {
    vector<BasicIf> e = s.else_ifs;
    for (int i = 0; i < e.size(); i++) {
      e[i].condition.accept(*this);
      var_table.push_environment();
      curr_frame.instructions.push_back(VMInstr::JMPF(-1));
      jmpf_index = curr_frame.instructions.size() - 1;
      for (auto &eis : e[i].stmts) 
        eis->accept(*this);
      var_table.pop_environment();
      // jump to end
      curr_frame.instructions.push_back(VMInstr::JMP(-1));
      jumps.push_back(curr_frame.instructions.size() - 1);
      curr_frame.instructions[jmpf_index] = VMInstr::JMPF(curr_frame.instructions.size());
    }
  }
  // else part
  if (!s.else_stmts.empty()) {
    var_table.push_environment();
    for (auto& es : s.else_stmts)
      es->accept(*this);
    var_table.pop_environment();
  }
  
  // update jmp indices with next stmt instr
  for (int i : jumps)
    curr_frame.instructions[i] = VMInstr::JMP(curr_frame.instructions.size());
}


void CodeGenerator::visit(VarDeclStmt& s)
{
  var_table.add(s.var_def.var_name.lexeme());
  s.expr.accept(*this);
  int index = var_table.get(s.var_def.var_name.lexeme());
  curr_frame.instructions.push_back(VMInstr::STORE(index));
}


void CodeGenerator::visit(AssignStmt& s)
{
  // load first var from table
  int index = var_table.get(s.lvalue[0].var_name.lexeme());
  curr_frame.instructions.push_back(VMInstr::LOAD(index));
  // array first var, get index
  if (s.lvalue[0].array_expr.has_value())
    s.lvalue[0].array_expr->accept(*this);

  // if path, loop till end
  if (s.lvalue.size() > 1)
  {
    if (s.lvalue[0].array_expr.has_value())
      curr_frame.instructions.push_back(VMInstr::GETI());
    for (int i = 1; i < s.lvalue.size() - 1; i++) {
      curr_frame.instructions.push_back(VMInstr::GETF(s.lvalue[i].var_name.lexeme()));
      if (s.lvalue[i].array_expr.has_value()) {
        s.lvalue[i].array_expr->accept(*this);
        curr_frame.instructions.push_back(VMInstr::GETI());
      }
    }

    // set last var in path with rhs
    VarRef last = s.lvalue.back();
    if (last.array_expr.has_value()) {
      curr_frame.instructions.push_back(VMInstr::GETF(last.var_name.lexeme()));
      last.array_expr->accept(*this);

      s.expr.accept(*this);
      curr_frame.instructions.push_back(VMInstr::SETI());
    }
    else {
      s.expr.accept(*this);
      curr_frame.instructions.push_back(VMInstr::SETF(last.var_name.lexeme()));
    }
  }
  else {
    s.expr.accept(*this);
    if (s.lvalue[0].array_expr.has_value())
      curr_frame.instructions.push_back(VMInstr::SETI());
    else
      curr_frame.instructions.push_back(VMInstr::STORE(index));
  }
}


void CodeGenerator::visit(CallExpr& e)
{
  // push args
  for (auto& arg : e.args)
    arg.accept(*this);
  // check built-ins
  string name = e.fun_name.lexeme();
  if (name == "print")
    curr_frame.instructions.push_back(VMInstr::WRITE());
  else if (name == "input")
    curr_frame.instructions.push_back(VMInstr::READ());
  else if (name == "to_string")
    curr_frame.instructions.push_back(VMInstr::TOSTR());
  else if (name == "to_int")
    curr_frame.instructions.push_back(VMInstr::TOINT());
  else if (name == "to_double")
    curr_frame.instructions.push_back(VMInstr::TODBL());
  else if (name == "length")
    curr_frame.instructions.push_back(VMInstr::SLEN());
  else if (name == "length@array")
    curr_frame.instructions.push_back(VMInstr::ALEN());
  else if (name == "get")
    curr_frame.instructions.push_back(VMInstr::GETC());
  else if (name == "concat")
    curr_frame.instructions.push_back(VMInstr::CONCAT());
  else
    curr_frame.instructions.push_back(VMInstr::CALL(name));
}


void CodeGenerator::visit(Expr& e)
{
  // push first part
  if (e.first.get() != nullptr)
    e.first->accept(*this);
  // push rest
  if (e.rest.get() == nullptr) {
    if (e.negated)
      curr_frame.instructions.push_back(VMInstr::NOT());
    return;
  }
  else
    e.rest->accept(*this);
  // push op
  if (e.op.has_value()) {
    if (e.op->type() == TokenType::PLUS)
      curr_frame.instructions.push_back(VMInstr::ADD());
    else if (e.op->type() == TokenType::MINUS)
      curr_frame.instructions.push_back(VMInstr::SUB());
    else if (e.op->type() == TokenType::TIMES)
      curr_frame.instructions.push_back(VMInstr::MUL());
    else if (e.op->type() == TokenType::DIVIDE)
      curr_frame.instructions.push_back(VMInstr::DIV());
    else if (e.op->type() == TokenType::AND)
      curr_frame.instructions.push_back(VMInstr::AND());
    else if (e.op->type() == TokenType::OR)
      curr_frame.instructions.push_back(VMInstr::OR());
    else if (e.op->type() == TokenType::LESS)
      curr_frame.instructions.push_back(VMInstr::CMPLT());
    else if (e.op->type() == TokenType::LESS_EQ)
      curr_frame.instructions.push_back(VMInstr::CMPLE());
    else if (e.op->type() == TokenType::GREATER)
      curr_frame.instructions.push_back(VMInstr::CMPGT());
    else if (e.op->type() == TokenType::GREATER_EQ)
      curr_frame.instructions.push_back(VMInstr::CMPGE());
    else if (e.op->type() == TokenType::EQUAL)
      curr_frame.instructions.push_back(VMInstr::CMPEQ());
    else if (e.op->type() == TokenType::NOT_EQUAL)
      curr_frame.instructions.push_back(VMInstr::CMPNE());
  }
  // negation
  if (e.negated)
    curr_frame.instructions.push_back(VMInstr::NOT());
}


void CodeGenerator::visit(SimpleTerm& t)
{
  // store rvalue
  t.rvalue->accept(*this);
}


void CodeGenerator::visit(ComplexTerm& t)
{
  // store rvalue
  t.expr.accept(*this);
}


void CodeGenerator::visit(SimpleRValue& v)
{
  if (v.value.type() == TokenType::INT_VAL) {
    int val = stoi(v.value.lexeme());
    curr_frame.instructions.push_back(VMInstr::PUSH(val));
  }
  else if (v.value.type() == TokenType::DOUBLE_VAL)
  {
    double val = stod(v.value.lexeme());
    curr_frame.instructions.push_back(VMInstr::PUSH(val));
  }
  else if (v.value.type() == TokenType::NULL_VAL)
    curr_frame.instructions.push_back(VMInstr::PUSH(nullptr));
  else if (v.value.type() == TokenType::BOOL_VAL)
  {
    if (v.value.lexeme() == "true")
      curr_frame.instructions.push_back(VMInstr::PUSH(true));
    else
      curr_frame.instructions.push_back(VMInstr::PUSH(false));
  }
  else if (v.value.type() == TokenType::STRING_VAL || 
          v.value.type() == TokenType::CHAR_VAL)
  {
    string s = v.value.lexeme();
    replace_all(s, "\\n", "\n");
    replace_all(s, "\\t", "\t");
    // added conditions
    replace_all(s, "\\r", "\r");
    replace_all(s, "\\'", "\'");
    curr_frame.instructions.push_back(VMInstr::PUSH(s));
  }
}


void CodeGenerator::visit(NewRValue& v)
{
  if (v.array_expr.has_value()) {
    // initialize array
    v.array_expr->accept(*this);
    curr_frame.instructions.push_back(VMInstr::PUSH(nullptr));
    curr_frame.instructions.push_back(VMInstr::ALLOCA());
  }
  else {
    curr_frame.instructions.push_back(VMInstr::ALLOCS());
    // initialize fields
    for (auto f : struct_defs[v.type.lexeme()].fields) {
      curr_frame.instructions.push_back(VMInstr::DUP());
      curr_frame.instructions.push_back(VMInstr::ADDF(f.var_name.lexeme()));
      curr_frame.instructions.push_back(VMInstr::DUP());
      curr_frame.instructions.push_back(VMInstr::PUSH(nullptr));
      curr_frame.instructions.push_back(VMInstr::SETF(f.var_name.lexeme()));
    }
  }
}


void CodeGenerator::visit(VarRValue& v)
{
  // get reference of first var in path
  int index = var_table.get(v.path[0].var_name.lexeme());
  curr_frame.instructions.push_back(VMInstr::LOAD(index));
  if (v.path[0].array_expr.has_value()) {
    v.path[0].array_expr->accept(*this);
    curr_frame.instructions.push_back(VMInstr::GETI());
  }
  // for next vars in path, add access to instrs
  for (int i = 1; i < v.path.size(); i++) {
    curr_frame.instructions.push_back(VMInstr::GETF(v.path[i].var_name.lexeme()));
    if (v.path[i].array_expr.has_value()) {
      v.path[i].array_expr->accept(*this);
      curr_frame.instructions.push_back(VMInstr::GETI());
    }
  }
}
