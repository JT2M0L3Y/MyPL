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
#include "ast_parser.h"
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
// Basic Structure tests
//----------------------------------------------------------------------

TEST(ExtensionTest, EmptyProgram) {
  stringstream in("void main() {}");
  VM vm;
  CodeGenerator generator(vm);
  ASTParser(Lexer(in)).parse().accept(generator);
  vm.run();
}

TEST(ExtensionTest, DictCreation) {
  stringstream in(build_string({
    "void main() {",
    "  dict<int, string> ps = new <int, string>",
    "  print(ps)",
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

TEST(ExtensionTest, DictInsertPair)
{
  stringstream in(build_string({
    "void main() {",
    "  dict<int, string> ps = new <int, string>",
    "  ps.insert(\"first\", 42)",
    "  print(ps[\"first\"])",
    "}"
  }));
  VM vm;
  CodeGenerator generator(vm);
  ASTParser(Lexer(in)).parse().accept(generator);
  stringstream out;
  change_cout(out);
  vm.run();
  EXPECT_EQ("42", out.str());
  restore_cout();
}

TEST(ExtensionTest, DictUpdatePair)
{
  stringstream in(build_string({
    "void main() {",
    "  dict<string, int> ps = new <int, string>",
    "  ps.insert(\"first\", 42)",
    "  ps[\"first\"] = 0",
    "  print(ps[\"first\"])",
    "}"
  }));
  VM vm;
  CodeGenerator generator(vm);
  ASTParser(Lexer(in)).parse().accept(generator);
  stringstream out;
  change_cout(out);
  vm.run();
  EXPECT_EQ("0", out.str());
  restore_cout();
}

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
