//----------------------------------------------------------------------
// FILE: mypl.cpp
// DATE: Spring 2023
// AUTH: Jonathan Smoley
// DESC: Homework 7 MyPL Source
//----------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <string_view>
#include "lexer.h"
#include "token.h"
#include "simple_parser.h"
#include "ast_parser.h"
#include "print_visitor.h"
#include "semantic_checker.h"
#include "vm.h"
#include "code_generator.h"

using namespace std;

void displayUsage();

enum class Mode
{
  DEFAULT,
  LEX,
  PARSE,
  PRINT,
  CHECK,
  IR,
  HELP,
  INVALID
};

Mode parse_mode(const vector<string> &args)
{
  if (args.size() < 2)
    return Mode::DEFAULT;

  const string_view flag = args[1];
  if (flag == "--lex")
    return Mode::LEX;
  if (flag == "--parse")
    return Mode::PARSE;
  if (flag == "--print")
    return Mode::PRINT;
  if (flag == "--check")
    return Mode::CHECK;
  if (flag == "--ir")
    return Mode::IR;
  if (flag == "--help")
    return Mode::HELP;

  if (args.size() == 2)
    return Mode::DEFAULT;

  return Mode::INVALID;
}

int main(int argc, char *argv[])
{
  try
  {
    vector<string> args;
    args.reserve(argc);
    for (int i = 0; i < argc; ++i)
      args.emplace_back(argv[i]);

    Mode mode = parse_mode(args);
    if (mode == Mode::HELP || mode == Mode::INVALID || argc > 3)
    {
      displayUsage();
      return 1;
    }

    const string file_arg = [&]()
    {
      if (argc == 2 && mode == Mode::DEFAULT)
        return args[1];
      if (argc == 3 && mode != Mode::DEFAULT)
        return args[2];
      return string();
    }();

    std::ifstream file_stream;
    std::istream *input = &cin;
    if (!file_arg.empty())
    {
      file_stream.open(file_arg);
      if (file_stream.fail())
        throw MyPLException("unable to open file: " + file_arg);
      input = &file_stream;
    }

    Lexer lexer(*input);

    if (mode == Mode::LEX)
    {
      Token t = lexer.next_token();
      cout << to_string(t) << endl;
      while (t.type() != TokenType::EOS)
      {
        t = lexer.next_token();
        cout << to_string(t) << endl;
      }
    }
    else if (mode == Mode::PARSE)
    {
      SimpleParser parser(lexer);
      parser.parse();
    }
    else if (mode == Mode::PRINT)
    {
      ASTParser parser(lexer);
      Program p = parser.parse();
      PrintVisitor printer(cout);
      p.accept(printer);
    }
    else if (mode == Mode::CHECK)
    {
      ASTParser parser(lexer);
      Program p = parser.parse();
      SemanticChecker checker;
      p.accept(checker);
    }
    else if (mode == Mode::IR)
    {
      ASTParser parser(lexer);
      Program p = parser.parse();
      SemanticChecker checker;
      p.accept(checker);
      VM vm;
      CodeGenerator g(vm);
      p.accept(g);
      cout << to_string(vm) << endl;
    }
    else
    {
      ASTParser parser(lexer);
      Program p = parser.parse();
      SemanticChecker checker;
      p.accept(checker);
      VM vm;
      CodeGenerator g(vm);
      p.accept(g);
      vm.run();
    }
  }
  catch (MyPLException &ex)
  {
    cerr << ex.what() << endl;
    return 1;
  }

  return 0;
}

/**
 * @brief Displays the usage information for the mypl interpreter.
 * @author Jonathan Smoley
 */
void displayUsage(void)
{
  cout << "Usage: ./mypl [option] [script-file]" << endl;
  cout << "Options:" << endl;
  cout << "  --help    prints this message" << endl;
  cout << "  --lex     displays token information" << endl;
  cout << "  --parse   checks for syntax errors" << endl;
  cout << "  --print   pretty prints program" << endl;
  cout << "  --check   statically checks program" << endl;
  cout << "  --ir      prints intermediate (code) representation" << endl;
}
