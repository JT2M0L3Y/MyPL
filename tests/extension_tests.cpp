//----------------------------------------------------------------------
// FILE: extension_tests.cpp
// DATE: CPSC 326, Spring 2023
// AUTH: Jonathan Smoley
// DESC: Extension project tests
//----------------------------------------------------------------------

#include <iostream>
#include <sstream>
#include <string>
#include <gtest/gtest.h>
#include "mypl_exception.h"
#include "lexer.h"
#include "simple_parser.h"
#include "ast_parser.h"
#include "semantic_checker.h"
#include "vm.h"
#include "code_generator.h"

using namespace std;

streambuf *stream_buffer;

void change_cout(stringstream &out)
{
  stream_buffer = cout.rdbuf();
  cout.rdbuf(out.rdbuf());
}

void restore_cout()
{
  cout.rdbuf(stream_buffer);
}

string build_string(initializer_list<string> strs)
{
  string result = "";
  for (string s : strs)
    result += s + "\n";
  return result;
}

//----------------------------------------------------------------------
// Lexer Tests
//----------------------------------------------------------------------

TEST(LexerTests, DictReservedWord)
{
  stringstream in("dict");
  Lexer lexer(in);
  Token t = lexer.next_token();
  ASSERT_EQ(TokenType::DICT, t.type());
  ASSERT_EQ("dict", t.lexeme());
  ASSERT_EQ(1, t.line());
  ASSERT_EQ(1, t.column());
  t = lexer.next_token();
  ASSERT_EQ(TokenType::EOS, t.type());
}

TEST(LexerTests, VarDeclWords)
{
  stringstream in("dict string double kvs");
  Lexer lexer(in);
  Token t = lexer.next_token();
  ASSERT_EQ(TokenType::DICT, t.type());
  ASSERT_EQ("dict", t.lexeme());
  ASSERT_EQ(1, t.line());
  ASSERT_EQ(1, t.column());
  t = lexer.next_token();
  ASSERT_EQ(TokenType::STRING_TYPE, t.type());
  ASSERT_EQ("string", t.lexeme());
  ASSERT_EQ(1, t.line());
  ASSERT_EQ(6, t.column());
  t = lexer.next_token();
  ASSERT_EQ(TokenType::DOUBLE_TYPE, t.type());
  ASSERT_EQ("double", t.lexeme());
  ASSERT_EQ(1, t.line());
  ASSERT_EQ(13, t.column());
  t = lexer.next_token();
  ASSERT_EQ(TokenType::ID, t.type());
  ASSERT_EQ("kvs", t.lexeme());
  ASSERT_EQ(1, t.line());
  ASSERT_EQ(20, t.column());
  t = lexer.next_token();
  ASSERT_EQ(TokenType::EOS, t.type());
}

TEST(LexerTests, AllocationWords)
{
  stringstream in("new dict{string, int}");
  Lexer lexer(in);
  Token t = lexer.next_token();
  ASSERT_EQ(TokenType::NEW, t.type());
  ASSERT_EQ("new", t.lexeme());
  ASSERT_EQ(1, t.line());
  ASSERT_EQ(1, t.column());
  t = lexer.next_token();
  ASSERT_EQ(TokenType::DICT, t.type());
  ASSERT_EQ("dict", t.lexeme());
  ASSERT_EQ(1, t.line());
  ASSERT_EQ(5, t.column());
  t = lexer.next_token();
  ASSERT_EQ(TokenType::STRING_TYPE, t.type());
  ASSERT_EQ("string", t.lexeme());
  ASSERT_EQ(1, t.line());
  ASSERT_EQ(10, t.column());
  t = lexer.next_token();
  ASSERT_EQ(TokenType::INT_TYPE, t.type());
  ASSERT_EQ("int", t.lexeme());
  ASSERT_EQ(1, t.line());
  ASSERT_EQ(18, t.column());
  t = lexer.next_token();
  ASSERT_EQ(TokenType::EOS, t.type());
}

//----------------------------------------------------------------------
// Simple Parser Tests
//----------------------------------------------------------------------

/*
TEST(SimpleParserTests, SimpleCreation)
{
  stringstream in(build_string({
      "dict string bool kvs = new dict{string, bool}"
    }));
  SimpleParser(Lexer(in)).parse();
}

TEST(SimpleParserTests, SimpleInsert)
{
  stringstream in(build_string({
      "kvs[\"foo\"] = true"
    }));
  SimpleParser(Lexer(in)).parse();
}

TEST(SimpleParserTests, SimpleAccess)
{
  stringstream in(build_string({
      "bool x = kvs[\"foo\"]"
    }));
  SimpleParser(Lexer(in)).parse();
}
*/

//----------------------------------------------------------------------
// AST Parser Tests
//----------------------------------------------------------------------

TEST(ASTParserTests, ASTDictVarDecl)
{
  stringstream in(build_string({
        "void main() {",
        "  dict string char kvs = new dict{string, char}",
        "}"
      }));
  Program p = ASTParser(Lexer(in)).parse();
  ASSERT_EQ(1, p.fun_defs[0].stmts.size());
  VarDeclStmt &s = (VarDeclStmt &)*p.fun_defs[0].stmts[0];
  ASSERT_TRUE(s.var_def.data_type.is_dict);
  ASSERT_EQ("string", s.var_def.data_type.type_names[0]);
  ASSERT_EQ("char", s.var_def.data_type.type_names[1]);
  ASSERT_EQ("kvs", s.var_def.var_name.lexeme());
}

TEST(ASTParserTests, ASTDictAssignment)
{
  stringstream in(build_string({
      "void main() {",
      "  kvs[\"foo\"] = true",
      "}"
    }));
  Program p = ASTParser(Lexer(in)).parse();
  AssignStmt &s = (AssignStmt &)*p.fun_defs[0].stmts[0];
  ASSERT_EQ(1, s.lvalue.size());
  ASSERT_EQ("kvs", s.lvalue[0].var_name.lexeme());
  ASSERT_TRUE(s.lvalue[0].dict_expr.has_value());
}

TEST(ASTParserTests, ASTDictValue)
{
  stringstream in(build_string({
      "void main() {",
      "  x = kvs[\"foo\"]",
      "}"
    }));
  Program p = ASTParser(Lexer(in)).parse();
  Expr &e = ((AssignStmt &)*p.fun_defs[0].stmts[0]).expr;
  VarRValue &v = (VarRValue &)*((SimpleTerm &)*e.first).rvalue;
  ASSERT_EQ(1, v.path.size());
  ASSERT_EQ("kvs", v.path[0].var_name.lexeme());
  ASSERT_TRUE(v.path[0].dict_expr.has_value());
}

//----------------------------------------------------------------------
// Parser Syntax Tests
//----------------------------------------------------------------------

TEST(ParserSyntaxTests, ParseDictCreation)
{
  stringstream in(build_string({
      "void my_fun() {",
      "  dict string string kv1 = new dict{string, string}",
      "  dict string int kv2 = new dict{string, int}",
      "  dict string bool kv3 = new dict{string, bool}",
      "  dict string char kv4 = new dict{string, char}",
      "  dict string double kv5 = new dict{string, double}",
      "  dict int string kv6 = new dict{int, string}",
      "  dict int int kv7 = new dict{int, int}",
      "  dict int bool kv8 = new dict{int, bool}",
      "  dict int char kv9 = new dict{int, char}",
      "  dict int double kv10 = new dict{int, double}",
      "}"
    }));
  ASTParser(Lexer(in)).parse();
}

TEST(ParserSyntaxTests, ParseDictAssign)
{
  stringstream in(build_string({
      "void my_fun() {",
      "  a[\"foo\"] = true",
      "  a[\"foo\"] = \"bar\"",
      "  a[\"foo\"] = 3.14",
      "  a[\"foo\"] = 5",
      "  a[\"foo\"] = 'a'",
      "  a[\"foo\"] = null",
      "  a[4] = false",
      "  a[4] = \"foo\"",
      "  a[4] = 1.25",
      "  a[4] = 17",
      "  a[4] = 'z'",
      "  a[4] = null",
      "}"
    }));
  ASTParser(Lexer(in)).parse();
}

TEST(ParserSyntaxTests, ParseDictValue)
{
  stringstream in(build_string({
      "void my_fun() {",
      "  int x = a[\"foo\"]",
      "  string x = a[\"foo\"]",
      "  bool x = a[\"foo\"]",
      "  char x = a[\"foo\"]",
      "  double x = a[\"foo\"]",
      "  int x = a[4]",
      "  string x =a[4]",
      "  bool x = a[4]",
      "  char x = a[4]",
      "  double x = a[4]",
      "}"
    }));
  ASTParser(Lexer(in)).parse();
}

//----------------------------------------------------------------------
// Semantic Checker Tests
//----------------------------------------------------------------------

/*
TEST(SemanticCheckerTests, StaticDictCreation)
{
  stringstream in(build_string({
      "void main() {",
      "  dict int string kvs = new dict{int, string}",
      "}"
    }));
  SemanticChecker checker;
  ASTParser(Lexer(in)).parse().accept(checker);
}

TEST(SemanticCheckerTests, StaticIfDictBoolValue)
{
  stringstream in(build_string({
      "void main() {",
      "  dict string int kvs = new dict{string, int}",
      "  if (5 < kvs[\"foo\"]) {}"
      "}"
    }));
  SemanticChecker checker;
  ASTParser(Lexer(in)).parse().accept(checker);
}

TEST(SemanticCheckerTests, StaticLoopDictBuiltIns)
{
  stringstream in(build_string({
      "void main() {",
      "  dict string int kvs = new dict{string, int}",
      "  kvs[\"foo\"] = 5",
      "  kvs[\"bar\"] = 4",
      "  for(int i = 0; i < len(keys(kvs)); i = i + 1) {",
      "     print(keys(kvs)[i])",
      "  }",
      "  for(int i = 0; i < len(values(kvs)); i = i + 1) {"
      "     print(values(kvs)[i])",
      "  }",
      "}"
    }));
  SemanticChecker checker;
  ASTParser(Lexer(in)).parse().accept(checker);
}
*/

//----------------------------------------------------------------------
// VM Tests
//----------------------------------------------------------------------

TEST(VMTests, VMDictAlloc)
{
  VMFrameInfo main{"main", 0};
  // add instructions
  VM vm;
  vm.add(main);
  stringstream out;
  change_cout(out);
  vm.run();
  // EXPECT_EQ("2023 2024", out.str());
  restore_cout();
}

TEST(VMTests, VMDictAccess)
{
  VMFrameInfo main{"main", 0};
  // add instructions
  VM vm;
  vm.add(main);
  stringstream out;
  change_cout(out);
  vm.run();
  // EXPECT_EQ("10", out.str());
  restore_cout();
}

TEST(VMTests, VMDictValue)
{
  VMFrameInfo main{"main", 0};
  // add instructions
  VM vm;
  vm.add(main);
  stringstream out;
  change_cout(out);
  vm.run();
  // EXPECT_EQ("0", out.str());
  restore_cout();
}

TEST(VMTests, DictKeysIndexing)
{
  VMFrameInfo main{"main", 0};
  // add instructions
  VM vm;
  vm.add(main);
  stringstream out;
  change_cout(out);
  vm.run();
  // EXPECT_EQ("10", out.str());
  restore_cout();
}

TEST(VMTests, DictValuesIndexing)
{
  VMFrameInfo main{"main", 0};
  // add instructions
  VM vm;
  vm.add(main);
  stringstream out;
  change_cout(out);
  vm.run();
  // EXPECT_EQ("10", out.str());
  restore_cout();
}

//----------------------------------------------------------------------
// Code Gen Tests
//----------------------------------------------------------------------

TEST(CodeGenTests, DictCreation) {
  stringstream in(build_string({
    "void main() {",
    "  dict int string kvs = new dict{int, string}",
    "  print(kvs)",
    "}"
  }));
  VM vm;
  CodeGenerator generator(vm);
  ASTParser(Lexer(in)).parse().accept(generator);
  stringstream out;
  change_cout(out);
  vm.run();
  EXPECT_EQ("2023", out.str());
  restore_cout();
}

TEST(CodeGenTests, DictInsertPair)
{
  stringstream in(build_string({
    "void main() {",
    "  dict int string kvs = new dict{int, string}",
    "  kvs[0] = \"first\"",
    "  print(kvs[0])",
    "}"
  }));
  VM vm;
  CodeGenerator generator(vm);
  ASTParser(Lexer(in)).parse().accept(generator);
  stringstream out;
  change_cout(out);
  vm.run();
  EXPECT_EQ("first", out.str());
  restore_cout();
}

TEST(CodeGenTests, DictUpdatePair)
{
  stringstream in(build_string({
    "void main() {",
    "  dict string int kvs = new dict{string, int}",
    "  kvs[\"first\"] = 42",
    "  print(kvs[\"first\"])",
    "  kvs[\"first\"] = 0",
    "  print(kvs[\"first\"])",
    "}"
  }));
  VM vm;
  CodeGenerator generator(vm);
  ASTParser(Lexer(in)).parse().accept(generator);
  stringstream out;
  change_cout(out);
  vm.run();
  EXPECT_EQ("420", out.str());
  restore_cout();
}

//----------------------------------------------------------------------
// Negative Tests (x7)
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
