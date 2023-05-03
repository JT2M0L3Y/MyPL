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
// Initialization tests
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Update tests
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Access tests
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Param tests
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Built-in tests
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
