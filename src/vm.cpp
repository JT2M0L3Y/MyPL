//----------------------------------------------------------------------
// FILE: vm.cpp
// DATE: CPSC 326, Spring 2023
// AUTH: Jonathan Smoley
// DESC: The mypl vm implementation
//----------------------------------------------------------------------

#include <iostream>
#include <unordered_map>
#include "vm.h"
#include "mypl_exception.h"


using namespace std;


void VM::error(string msg) const
{
  throw MyPLException::VMError(msg);
}


void VM::error(string msg, const VMFrame& frame) const
{
  int pc = frame.pc - 1;
  VMInstr instr = frame.info.instructions[pc];
  string name = frame.info.function_name;
  msg += " (in " + name + " at " + to_string(pc) + ": " +
    to_string(instr) + ")";
  throw MyPLException::VMError(msg);
}


string to_string(const VM& vm)
{
  string s = "";
  for (const auto& entry : vm.frame_info) {
    const string& name = entry.first;
    s += "\nFrame '" + name + "'\n";
    const VMFrameInfo& frame = entry.second;
    for (int i = 0; i < frame.instructions.size(); ++i) {
      VMInstr instr = frame.instructions[i];
      s += "  " + to_string(i) + ": " + to_string(instr) + "\n"; 
    }
  }
  return s;
}


void VM::add(const VMFrameInfo& frame)
{
  frame_info[frame.function_name] = frame;
}


void VM::run(bool DEBUG)
{
  // grab the "main" frame if it exists
  if (!frame_info.contains("main"))
    error("No 'main' function");
  shared_ptr<VMFrame> frame = make_shared<VMFrame>();
  frame->info = frame_info["main"];
  call_stack.push(frame);

  // run loop (keep going until we run out of instructions)
  while (!call_stack.empty() and frame->pc < frame->info.instructions.size()) 
  {
    // get the next instruction
    VMInstr& instr = frame->info.instructions[frame->pc];
    // increment the program counter
    ++frame->pc;
    // for debugging
    if (DEBUG) {
      cerr << endl << endl;
      cerr << "\t FRAME.........: " << frame->info.function_name << endl;
      cerr << "\t PC............: " << (frame->pc - 1) << endl;
      cerr << "\t INSTR.........: " << to_string(instr) << endl;
      cerr << "\t NEXT OPERAND..: ";
      if (!frame->operand_stack.empty())
        cerr << to_string(frame->operand_stack.top()) << endl;
      else
        cerr << "empty" << endl;
      cerr << "\t NEXT FUNCTION.: ";
      if (!call_stack.empty())
        cerr << call_stack.top()->info.function_name << endl;
      else
        cerr << "empty" << endl;
    }

    //----------------------------------------------------------------------
    // Literals and Variables
    //----------------------------------------------------------------------

    if (instr.opcode() == OpCode::PUSH)
      frame->operand_stack.push(instr.operand().value());
    else if (instr.opcode() == OpCode::POP)
      frame->operand_stack.pop();
    else if (instr.opcode() == OpCode::LOAD) {
      // get v at memory address i, push v onto stack
      frame->operand_stack.push(frame->variables[get<int>(instr.operand().value())]);
    }
    else if (instr.opcode() == OpCode::STORE) {
      VMValue v = frame->operand_stack.top();
      if (!frame->variables.empty()) {
        if (get<int>(instr.operand().value()) >= frame->variables.size())
          frame->variables.push_back(v);
        else
          frame->variables[get<int>(instr.operand().value())] = v;
      }
      else
        frame->variables.push_back(v);
      frame->operand_stack.pop();
    }

    //----------------------------------------------------------------------
    // Operations
    //----------------------------------------------------------------------

    // Arithmetic operators
    else if (instr.opcode() == OpCode::ADD) {
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      frame->operand_stack.push(add(y, x));
    }
    else if (instr.opcode() == OpCode::SUB) {
      // pop x
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      // pop y
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      // push (y - x)
      frame->operand_stack.push(sub(y, x));
    }
    else if (instr.opcode() == OpCode::MUL) {
      // pop x
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      // pop y
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      // push (y * x)
      frame->operand_stack.push(mul(y, x));
    }
    else if (instr.opcode() == OpCode::DIV) {
      // pop x
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      // pop y
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      // push (y / x)
      frame->operand_stack.push(div(y, x));
    }
    // Logical operators
    else if (instr.opcode() == OpCode::AND) {
      // pop bool x
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      // pop bool y
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      // push (y and x)
      frame->operand_stack.push(get<bool>(y) and get<bool>(x));
    }
    else if (instr.opcode() == OpCode::OR) {
      // pop bool x
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      // pop bool y
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      // push (y or x)
      frame->operand_stack.push(get<bool>(y) or get<bool>(x));
    }
    else if (instr.opcode() == OpCode::NOT) {
      // pop bool x
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      // push (not x)
      frame->operand_stack.push(not get<bool>(x));
    }
    // Relational (comparison) operators
    else if (instr.opcode() == OpCode::CMPLT) {
      // pop x
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      // pop y
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      // push (y < x)
      frame->operand_stack.push(lt(y, x));
    }
    else if (instr.opcode() == OpCode::CMPLE) {
      // pop x
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      // pop y
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      // push (y <= x)
      frame->operand_stack.push(le(y, x));
    }
    else if (instr.opcode() == OpCode::CMPGT) {
      // pop x
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      // pop y
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      // push (y > x)
      frame->operand_stack.push(gt(y, x));
    }
    else if (instr.opcode() == OpCode::CMPGE) {
      // pop x
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      // pop y
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      // push (y >= x)
      frame->operand_stack.push(ge(y, x));
    }
    else if (instr.opcode() == OpCode::CMPEQ) {
      // pop x
      VMValue x = frame->operand_stack.top();
      frame->operand_stack.pop();
      // pop y
      VMValue y = frame->operand_stack.top();
      frame->operand_stack.pop();
      // push (y == x)
      frame->operand_stack.push(eq(y, x));
    }
    else if (instr.opcode() == OpCode::CMPNE) {
      // pop x
      VMValue x = frame->operand_stack.top();
      frame->operand_stack.pop();
      // pop y
      VMValue y = frame->operand_stack.top();
      frame->operand_stack.pop();
      // push (y != x)
      frame->operand_stack.push(!get<bool>(eq(y, x)));
    }

    //----------------------------------------------------------------------
    // Branching
    //----------------------------------------------------------------------
    
    else if (instr.opcode() == OpCode::JMP)
      frame->pc = get<int>(instr.operand().value());
    else if (instr.opcode() == OpCode::JMPF) {
      // pop x
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      // if x is false jump to instr i
      if (!get<bool>(x))
        frame->pc = get<int>(instr.operand().value());
    }

    //----------------------------------------------------------------------
    // Functions
    //----------------------------------------------------------------------

    else if (instr.opcode() == OpCode::CALL) {
      // call function f
      VMValue fun_name = instr.operand().value();
      shared_ptr<VMFrame> fun_frame = make_shared<VMFrame>();
      fun_frame->info = frame_info[get<string>(fun_name)];

      call_stack.push(fun_frame);
      // copy over function operands
      for (int i = 0; i < fun_frame->info.arg_count; i++) {
        VMValue x = frame->operand_stack.top();
        fun_frame->operand_stack.push(x);
        frame->operand_stack.pop();
      }
      frame = fun_frame;
    }
    else if (instr.opcode() == OpCode::RET) {
      // store return value, remove fxn
      VMValue ret = frame->operand_stack.top();
      call_stack.pop();
      // check if fxn still present
      if (!call_stack.empty()) {
        frame = call_stack.top();
        frame->operand_stack.push(ret);
      }
    }

    //----------------------------------------------------------------------
    // Built in functions
    //----------------------------------------------------------------------

    else if (instr.opcode() == OpCode::WRITE) {
      VMValue x = frame->operand_stack.top();
      frame->operand_stack.pop();
      cout << to_string(x);
    }
    else if (instr.opcode() == OpCode::READ) {
      string val = "";
      getline(cin, val);
      frame->operand_stack.push(val);
    }
    else if (instr.opcode() == OpCode::SLEN) {
      // pop string x
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      // push x.size()
      int x_size = get<string>(x).size();
      frame->operand_stack.push(x_size);
    }
    else if (instr.opcode() == OpCode::ALEN) {
      // pop array x
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      // push x.size()
      int x_size = array_heap[get<int>(x)].size();
      frame->operand_stack.push(x_size);
    }
    else if (instr.opcode() == OpCode::GETC) {
      // pop string x
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      // pop int y
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      if (get<int>(y) >= get<string>(x).size() || get<int>(y) < 0)
        error("out-of-bounds string index", *frame);
      // push x[y]
      frame->operand_stack.push(string(1, get<string>(x)[get<int>(y)]));
    }
    else if (instr.opcode() == OpCode::TOINT) {
      // pop x
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      // push x as integer
      if (holds_alternative<double>(x)) 
      {
        double dbl = get<double>(x);
        frame->operand_stack.push((int) dbl);
      } 
      else 
      {
        string str = get<string>(x);
        for (int i = 0; i < str.size(); ++i)
        {
          if (!isdigit(str[i]))
            error("cannot convert string to int", *frame);
        }
        frame->operand_stack.push(stoi(str));
      }
    }
    else if (instr.opcode() == OpCode::TODBL) {
      // pop x
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      // push x as double
      if (holds_alternative<int>(x))
      {
        int i = get<int>(x);
        frame->operand_stack.push((double) i);
      }
      else
      {
        string str = get<string>(x);
        int dec_count = 0;
        for (int i = 0; i < str.size(); ++i)
        {
          if (!isdigit(str[i]) && str[i] != '.')
            error("cannot convert string to double", *frame);
        }
        frame->operand_stack.push(stod(str));
      }
    }
    else if (instr.opcode() == OpCode::TOSTR) {
      // pop x
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      // push x as string
      frame->operand_stack.push(to_string(x));
    }
    else if (instr.opcode() == OpCode::CONCAT) {
      // pop x
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      // pop y
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      // push y + x as string
      frame->operand_stack.push(get<string>(y) + get<string>(x));
    }
    else if (instr.opcode() == OpCode::KEYS) {
      // pop x
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      // push obj([obj(x).keys]) (i.e., array oid for keys array)
      std::unordered_map<VMValue, VMValue> d = dict_heap[get<int>(x)];
      std::vector<VMValue> ks;
      for (auto p : d)
        ks.push_back(p.first);
      array_heap[next_obj_id] = ks;
      frame->operand_stack.push(next_obj_id);
      ++next_obj_id;
    }
    else if (instr.opcode() == OpCode::VALUES) {
      // pop x
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      // push obj([obj(x).values]) (i.e., array oid for values array)
      std::unordered_map<VMValue, VMValue> d = dict_heap[get<int>(x)];
      std::vector<VMValue> ks;
      for (auto p : d)
        ks.push_back(p.second);
      array_heap[next_obj_id] = ks;
      frame->operand_stack.push(next_obj_id);
      ++next_obj_id;
    }

    //----------------------------------------------------------------------
    // heap
    //----------------------------------------------------------------------

    else if (instr.opcode() == OpCode::ALLOCS) {
      // allocate struct object in heap
      struct_heap[next_obj_id] = {};
      // push oid x
      frame->operand_stack.push(next_obj_id);
      ++next_obj_id;
    }
    else if (instr.opcode() == OpCode::ALLOCA) {
      // pop x and y
      VMValue x = frame->operand_stack.top();
      frame->operand_stack.pop();
      int y = get<int>(frame->operand_stack.top());
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      // allocate array in heap with y x values
      array_heap[next_obj_id] = vector<VMValue>(y, x);
      // push oid z
      frame->operand_stack.push(next_obj_id);
      ++next_obj_id;
    }
    else if (instr.opcode() == OpCode::ALLOCD) {
      // pop x and y
      // VMValue x = frame->operand_stack.top();
      // frame->operand_stack.pop();
      // VMValue y = frame->operand_stack.top();
      // frame->operand_stack.pop();
      // allocate dict in heap with y-type keys and x-type values
      // dict_heap[next_obj_id] = {nullptr, nullptr};
      ++next_obj_id;
    }
    else if (instr.opcode() == OpCode::ADDF) {
      // pop oid x
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      // add field f to obj(x)
      struct_heap[get<int>(x)][get<string>(instr.operand().value())] = nullptr;
    }
    else if (instr.opcode() == OpCode::SETF) {
      // pop x and y
      VMValue x = frame->operand_stack.top();
      frame->operand_stack.pop();
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      // in heap set obj(y).f = x
      struct_heap[get<int>(y)][get<string>(instr.operand().value())] = x;
    }
    else if (instr.opcode() == OpCode::GETF) {
      // pop x
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      // push obj(x).f onto operand stack
      frame->operand_stack.push(struct_heap[get<int>(x)][get<string>(instr.operand().value())]);
    }
    else if (instr.opcode() == OpCode::SETI) {
      // pop x, y, z
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      VMValue z = frame->operand_stack.top();
      ensure_not_null(*frame, z);
      frame->operand_stack.pop();
      if (get<int>(y) >= array_heap[get<int>(z)].size() || get<int>(y) < 0)
        error("out-of-bounds array index", *frame);
      // set array obj(z)[y] = x
      array_heap[get<int>(z)][get<int>(y)] = x;
    }
    else if (instr.opcode() == OpCode::GETI) {
      // pop x and y
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      if (get<int>(x) >= array_heap[get<int>(y)].size() || get<int>(x) < 0)
        error("out-of-bounds array index", *frame);
      // push array obj(y)[x] value onto operand stack
      frame->operand_stack.push(array_heap[get<int>(y)][get<int>(x)]);
    }
    else if (instr.opcode() == OpCode::ADDKV) {
      // pop x
      VMValue x = frame->operand_stack.top();
      frame->operand_stack.pop();
      // pop y
      VMValue y = frame->operand_stack.top();
      frame->operand_stack.pop();
      // pop z
      VMValue z = frame->operand_stack.top();
      ensure_not_null(*frame, z);
      frame->operand_stack.pop();
      // add kv pair to obj(z)
      dict_heap[get<int>(z)].insert({y, x});
    }
    else if (instr.opcode() == OpCode::SETV) {
      // pop x
      VMValue x = frame->operand_stack.top();
      frame->operand_stack.pop();
      // pop y
      VMValue y = frame->operand_stack.top();
      frame->operand_stack.pop();
      dict_heap[get<int>(y)][instr.operand().value()] = x;
    }
    else if (instr.opcode() == OpCode::GETV) {
      // pop x
      VMValue x = frame->operand_stack.top();
      frame->operand_stack.pop();
      frame->operand_stack.push(dict_heap[get<int>(x)][instr.operand().value()]);
    }

    //----------------------------------------------------------------------
    // special
    //----------------------------------------------------------------------

    else if (instr.opcode() == OpCode::DUP) {
      VMValue x = frame->operand_stack.top();
      frame->operand_stack.pop();
      frame->operand_stack.push(x);
      frame->operand_stack.push(x);      
    }
    else if (instr.opcode() == OpCode::NOP) {
      // do nothing
    }
    else
      error("unsupported operation " + to_string(instr));
  }
}

// helper methods for operators

void VM::ensure_not_null(const VMFrame& f, const VMValue& x) const
{
  if (holds_alternative<nullptr_t>(x))
    error("null reference", f);
}

// arithmetic operators

VMValue VM::add(const VMValue& x, const VMValue& y) const
{
  if (holds_alternative<int>(x)) 
    return get<int>(x) + get<int>(y);
  else
    return get<double>(x) + get<double>(y);
}

VMValue VM::sub(const VMValue& x, const VMValue& y) const
{
  if (holds_alternative<int>(x))
    return get<int>(x) - get<int>(y);
  else
    return get<double>(x) - get<double>(y);
}

VMValue VM::mul(const VMValue& x, const VMValue& y) const
{
  if (holds_alternative<int>(x))
    return get<int>(x) * get<int>(y);
  else
    return get<double>(x) * get<double>(y);
}

VMValue VM::div(const VMValue& x, const VMValue& y) const
{
  if (holds_alternative<int>(x))
    return get<int>(x) / get<int>(y);
  else
    return get<double>(x) / get<double>(y);
}

// relational operators

VMValue VM::eq(const VMValue& x, const VMValue& y) const
{
  if (holds_alternative<nullptr_t>(x) and not holds_alternative<nullptr_t>(y)) 
    return false;
  else if (not holds_alternative<nullptr_t>(x) and holds_alternative<nullptr_t>(y))
    return false;
  else if (holds_alternative<nullptr_t>(x) and holds_alternative<nullptr_t>(y))
    return true;
  else if (holds_alternative<int>(x)) 
    return get<int>(x) == get<int>(y);
  else if (holds_alternative<double>(x))
    return get<double>(x) == get<double>(y);
  else if (holds_alternative<string>(x))
    return get<string>(x) == get<string>(y);
  else
    return get<bool>(x) == get<bool>(y);
}

VMValue VM::lt(const VMValue& x, const VMValue& y) const
{
  if (holds_alternative<double>(x))
    return get<double>(x) < get<double>(y);
  else if (holds_alternative<string>(x))
    return get<string>(x) < get<string>(y);
  else
    return get<int>(x) < get<int>(y);
}

VMValue VM::le(const VMValue& x, const VMValue& y) const
{
  if (holds_alternative<double>(x))
    return get<double>(x) <= get<double>(y);
  else if (holds_alternative<string>(x))
    return get<string>(x) <= get<string>(y);
  else
    return get<int>(x) <= get<int>(y);
}

VMValue VM::gt(const VMValue& x, const VMValue& y) const
{
  if (holds_alternative<double>(x))
    return get<double>(x) > get<double>(y);
  else if (holds_alternative<string>(x))
    return get<string>(x) > get<string>(y);
  else
    return get<int>(x) > get<int>(y);
}

VMValue VM::ge(const VMValue& x, const VMValue& y) const
{
  if (holds_alternative<double>(x))
    return get<double>(x) >= get<double>(y);
  else if (holds_alternative<string>(x))
    return get<string>(x) >= get<string>(y);
  else
    return get<int>(x) >= get<int>(y);
}

