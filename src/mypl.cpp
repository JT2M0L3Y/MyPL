//----------------------------------------------------------------------
// FILE: mypl.cpp
// DATE: Spring 2023
// AUTH: Jonathan Smoley
// DESC: Homework 7 MyPL Source
//----------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <lexer.h>
#include <token.h>
#include <simple_parser.h>
#include <ast_parser.h>
#include <print_visitor.h>
#include <semantic_checker.h>
#include <vm.h>
#include <code_generator.h>

using namespace std;

void displayUsage(void);

int main(int argc, char *argv[])
{
  try
  {
    istream *input = &cin;
    string args[argc];
    bool lexMode = false;
    bool parseMode = false;
    bool printMode = false;
    bool checkMode = false;
    bool irMode = false;

    // convert argv to string array
    for (int i = 0; i < argc; i++)
    {
      args[i] = string(argv[i]);
    }

    // check correct number of args
    if (argc > 1 && argc < 4)
    {
      // check flag mode
      if (args[1] == "--lex")
      {
        lexMode = true;
      }
      else if (args[1] == "--parse")
      {
        parseMode = true;
      }
      else if (args[1] == "--print")
      {
        printMode = true;
      }
      else if (args[1] == "--check")
      {
        checkMode = true;
      }
      else if (args[1] == "--ir")
      {
        irMode = true;
      }
    }
    else
    {
      displayUsage();
      return 1;
    }

    // assign flag status
    bool flag = lexMode || parseMode || printMode || checkMode || irMode;

    // incorrect args
    if ((argc == 3 && !flag) || args[1] == "--help")
    {
      displayUsage();
      return 1;
    }

    // if necessary, modify input stream
    if ((argc == 3))
    {
      input = new ifstream(args[2]);

      if (input->fail())
      {
        throw MyPLException("unable to open file: " + args[2]);
      }
    }
    else if (argc == 2 && !flag)
    {
      input = new ifstream(args[1]);

      if (input->fail())
      {
        throw MyPLException("unable to open file: " + args[1]);
      }
    }

    Lexer lexer(*input);

    // run correct mode
    if (lexMode)
    {
      Token t = lexer.next_token();

      // lex tokens
      cout << to_string(t) << endl;
      while (t.type() != TokenType::EOS)
      {
        t = lexer.next_token();
        cout << to_string(t) << endl;
      }
    }
    else if (parseMode)
    {
      SimpleParser parser(lexer);

      // parse
      parser.parse();
    }
    else if (printMode)
    {
      ASTParser parser(lexer);
      Program p = parser.parse();
      PrintVisitor printer(cout);

      // check syntax
      p.accept(printer);
    }
    else if (checkMode)
    {
      ASTParser parser(lexer);
      Program p = parser.parse();
      SemanticChecker checker;

      // check semantics
      p.accept(checker);
    }
    else if (irMode)
    {
      ASTParser parser(lexer);
      Program p = parser.parse();
      SemanticChecker t;
      p.accept(t);
      VM vm;
      CodeGenerator g(vm);
      p.accept(g);
      cout << to_string(vm) << endl;
    }
    else
    {
      ASTParser parser(lexer);
      Program p = parser.parse();
      SemanticChecker t;
      p.accept(t);
      VM vm;
      CodeGenerator g(vm);
      p.accept(g);
      vm.run();
    }

    // free memory if necessary
    if ((argc == 3) || (argc == 2 && !flag))
    {
      delete input;
    }
  }
  catch (MyPLException &ex)
  {
    cerr << ex.what() << endl;
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
