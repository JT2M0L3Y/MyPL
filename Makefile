# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/mnt/c/Users/jtsmo/OneDrive - Gonzaga University/junior/spring/opl/hw7-JT2M0L3Y"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/c/Users/jtsmo/OneDrive - Gonzaga University/junior/spring/opl/hw7-JT2M0L3Y"

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start "/mnt/c/Users/jtsmo/OneDrive - Gonzaga University/junior/spring/opl/hw7-JT2M0L3Y/CMakeFiles" "/mnt/c/Users/jtsmo/OneDrive - Gonzaga University/junior/spring/opl/hw7-JT2M0L3Y//CMakeFiles/progress.marks"
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start "/mnt/c/Users/jtsmo/OneDrive - Gonzaga University/junior/spring/opl/hw7-JT2M0L3Y/CMakeFiles" 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named code_generator_tests

# Build rule for target.
code_generator_tests: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 code_generator_tests
.PHONY : code_generator_tests

# fast build rule for target.
code_generator_tests/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/build
.PHONY : code_generator_tests/fast

#=============================================================================
# Target rules for targets named mypl

# Build rule for target.
mypl: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 mypl
.PHONY : mypl

# fast build rule for target.
mypl/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/build
.PHONY : mypl/fast

src/ast_parser.o: src/ast_parser.cpp.o
.PHONY : src/ast_parser.o

# target to build an object file
src/ast_parser.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/src/ast_parser.cpp.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/ast_parser.cpp.o
.PHONY : src/ast_parser.cpp.o

src/ast_parser.i: src/ast_parser.cpp.i
.PHONY : src/ast_parser.i

# target to preprocess a source file
src/ast_parser.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/src/ast_parser.cpp.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/ast_parser.cpp.i
.PHONY : src/ast_parser.cpp.i

src/ast_parser.s: src/ast_parser.cpp.s
.PHONY : src/ast_parser.s

# target to generate assembly for a file
src/ast_parser.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/src/ast_parser.cpp.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/ast_parser.cpp.s
.PHONY : src/ast_parser.cpp.s

src/code_generator.o: src/code_generator.cpp.o
.PHONY : src/code_generator.o

# target to build an object file
src/code_generator.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/src/code_generator.cpp.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/code_generator.cpp.o
.PHONY : src/code_generator.cpp.o

src/code_generator.i: src/code_generator.cpp.i
.PHONY : src/code_generator.i

# target to preprocess a source file
src/code_generator.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/src/code_generator.cpp.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/code_generator.cpp.i
.PHONY : src/code_generator.cpp.i

src/code_generator.s: src/code_generator.cpp.s
.PHONY : src/code_generator.s

# target to generate assembly for a file
src/code_generator.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/src/code_generator.cpp.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/code_generator.cpp.s
.PHONY : src/code_generator.cpp.s

src/lexer.o: src/lexer.cpp.o
.PHONY : src/lexer.o

# target to build an object file
src/lexer.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/src/lexer.cpp.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/lexer.cpp.o
.PHONY : src/lexer.cpp.o

src/lexer.i: src/lexer.cpp.i
.PHONY : src/lexer.i

# target to preprocess a source file
src/lexer.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/src/lexer.cpp.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/lexer.cpp.i
.PHONY : src/lexer.cpp.i

src/lexer.s: src/lexer.cpp.s
.PHONY : src/lexer.s

# target to generate assembly for a file
src/lexer.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/src/lexer.cpp.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/lexer.cpp.s
.PHONY : src/lexer.cpp.s

src/mypl.o: src/mypl.cpp.o
.PHONY : src/mypl.o

# target to build an object file
src/mypl.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/mypl.cpp.o
.PHONY : src/mypl.cpp.o

src/mypl.i: src/mypl.cpp.i
.PHONY : src/mypl.i

# target to preprocess a source file
src/mypl.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/mypl.cpp.i
.PHONY : src/mypl.cpp.i

src/mypl.s: src/mypl.cpp.s
.PHONY : src/mypl.s

# target to generate assembly for a file
src/mypl.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/mypl.cpp.s
.PHONY : src/mypl.cpp.s

src/mypl_exception.o: src/mypl_exception.cpp.o
.PHONY : src/mypl_exception.o

# target to build an object file
src/mypl_exception.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/src/mypl_exception.cpp.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/mypl_exception.cpp.o
.PHONY : src/mypl_exception.cpp.o

src/mypl_exception.i: src/mypl_exception.cpp.i
.PHONY : src/mypl_exception.i

# target to preprocess a source file
src/mypl_exception.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/src/mypl_exception.cpp.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/mypl_exception.cpp.i
.PHONY : src/mypl_exception.cpp.i

src/mypl_exception.s: src/mypl_exception.cpp.s
.PHONY : src/mypl_exception.s

# target to generate assembly for a file
src/mypl_exception.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/src/mypl_exception.cpp.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/mypl_exception.cpp.s
.PHONY : src/mypl_exception.cpp.s

src/print_visitor.o: src/print_visitor.cpp.o
.PHONY : src/print_visitor.o

# target to build an object file
src/print_visitor.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/print_visitor.cpp.o
.PHONY : src/print_visitor.cpp.o

src/print_visitor.i: src/print_visitor.cpp.i
.PHONY : src/print_visitor.i

# target to preprocess a source file
src/print_visitor.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/print_visitor.cpp.i
.PHONY : src/print_visitor.cpp.i

src/print_visitor.s: src/print_visitor.cpp.s
.PHONY : src/print_visitor.s

# target to generate assembly for a file
src/print_visitor.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/print_visitor.cpp.s
.PHONY : src/print_visitor.cpp.s

src/semantic_checker.o: src/semantic_checker.cpp.o
.PHONY : src/semantic_checker.o

# target to build an object file
src/semantic_checker.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/semantic_checker.cpp.o
.PHONY : src/semantic_checker.cpp.o

src/semantic_checker.i: src/semantic_checker.cpp.i
.PHONY : src/semantic_checker.i

# target to preprocess a source file
src/semantic_checker.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/semantic_checker.cpp.i
.PHONY : src/semantic_checker.cpp.i

src/semantic_checker.s: src/semantic_checker.cpp.s
.PHONY : src/semantic_checker.s

# target to generate assembly for a file
src/semantic_checker.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/semantic_checker.cpp.s
.PHONY : src/semantic_checker.cpp.s

src/simple_parser.o: src/simple_parser.cpp.o
.PHONY : src/simple_parser.o

# target to build an object file
src/simple_parser.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/simple_parser.cpp.o
.PHONY : src/simple_parser.cpp.o

src/simple_parser.i: src/simple_parser.cpp.i
.PHONY : src/simple_parser.i

# target to preprocess a source file
src/simple_parser.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/simple_parser.cpp.i
.PHONY : src/simple_parser.cpp.i

src/simple_parser.s: src/simple_parser.cpp.s
.PHONY : src/simple_parser.s

# target to generate assembly for a file
src/simple_parser.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/simple_parser.cpp.s
.PHONY : src/simple_parser.cpp.s

src/symbol_table.o: src/symbol_table.cpp.o
.PHONY : src/symbol_table.o

# target to build an object file
src/symbol_table.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/symbol_table.cpp.o
.PHONY : src/symbol_table.cpp.o

src/symbol_table.i: src/symbol_table.cpp.i
.PHONY : src/symbol_table.i

# target to preprocess a source file
src/symbol_table.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/symbol_table.cpp.i
.PHONY : src/symbol_table.cpp.i

src/symbol_table.s: src/symbol_table.cpp.s
.PHONY : src/symbol_table.s

# target to generate assembly for a file
src/symbol_table.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/symbol_table.cpp.s
.PHONY : src/symbol_table.cpp.s

src/token.o: src/token.cpp.o
.PHONY : src/token.o

# target to build an object file
src/token.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/src/token.cpp.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/token.cpp.o
.PHONY : src/token.cpp.o

src/token.i: src/token.cpp.i
.PHONY : src/token.i

# target to preprocess a source file
src/token.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/src/token.cpp.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/token.cpp.i
.PHONY : src/token.cpp.i

src/token.s: src/token.cpp.s
.PHONY : src/token.s

# target to generate assembly for a file
src/token.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/src/token.cpp.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/token.cpp.s
.PHONY : src/token.cpp.s

src/var_table.o: src/var_table.cpp.o
.PHONY : src/var_table.o

# target to build an object file
src/var_table.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/src/var_table.cpp.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/var_table.cpp.o
.PHONY : src/var_table.cpp.o

src/var_table.i: src/var_table.cpp.i
.PHONY : src/var_table.i

# target to preprocess a source file
src/var_table.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/src/var_table.cpp.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/var_table.cpp.i
.PHONY : src/var_table.cpp.i

src/var_table.s: src/var_table.cpp.s
.PHONY : src/var_table.s

# target to generate assembly for a file
src/var_table.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/src/var_table.cpp.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/var_table.cpp.s
.PHONY : src/var_table.cpp.s

src/vm.o: src/vm.cpp.o
.PHONY : src/vm.o

# target to build an object file
src/vm.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/src/vm.cpp.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/vm.cpp.o
.PHONY : src/vm.cpp.o

src/vm.i: src/vm.cpp.i
.PHONY : src/vm.i

# target to preprocess a source file
src/vm.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/src/vm.cpp.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/vm.cpp.i
.PHONY : src/vm.cpp.i

src/vm.s: src/vm.cpp.s
.PHONY : src/vm.s

# target to generate assembly for a file
src/vm.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/src/vm.cpp.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/vm.cpp.s
.PHONY : src/vm.cpp.s

src/vm_instr.o: src/vm_instr.cpp.o
.PHONY : src/vm_instr.o

# target to build an object file
src/vm_instr.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/src/vm_instr.cpp.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/vm_instr.cpp.o
.PHONY : src/vm_instr.cpp.o

src/vm_instr.i: src/vm_instr.cpp.i
.PHONY : src/vm_instr.i

# target to preprocess a source file
src/vm_instr.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/src/vm_instr.cpp.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/vm_instr.cpp.i
.PHONY : src/vm_instr.cpp.i

src/vm_instr.s: src/vm_instr.cpp.s
.PHONY : src/vm_instr.s

# target to generate assembly for a file
src/vm_instr.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/src/vm_instr.cpp.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/mypl.dir/build.make CMakeFiles/mypl.dir/src/vm_instr.cpp.s
.PHONY : src/vm_instr.cpp.s

tests/code_generator_tests.o: tests/code_generator_tests.cpp.o
.PHONY : tests/code_generator_tests.o

# target to build an object file
tests/code_generator_tests.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/tests/code_generator_tests.cpp.o
.PHONY : tests/code_generator_tests.cpp.o

tests/code_generator_tests.i: tests/code_generator_tests.cpp.i
.PHONY : tests/code_generator_tests.i

# target to preprocess a source file
tests/code_generator_tests.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/tests/code_generator_tests.cpp.i
.PHONY : tests/code_generator_tests.cpp.i

tests/code_generator_tests.s: tests/code_generator_tests.cpp.s
.PHONY : tests/code_generator_tests.s

# target to generate assembly for a file
tests/code_generator_tests.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/code_generator_tests.dir/build.make CMakeFiles/code_generator_tests.dir/tests/code_generator_tests.cpp.s
.PHONY : tests/code_generator_tests.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... code_generator_tests"
	@echo "... mypl"
	@echo "... src/ast_parser.o"
	@echo "... src/ast_parser.i"
	@echo "... src/ast_parser.s"
	@echo "... src/code_generator.o"
	@echo "... src/code_generator.i"
	@echo "... src/code_generator.s"
	@echo "... src/lexer.o"
	@echo "... src/lexer.i"
	@echo "... src/lexer.s"
	@echo "... src/mypl.o"
	@echo "... src/mypl.i"
	@echo "... src/mypl.s"
	@echo "... src/mypl_exception.o"
	@echo "... src/mypl_exception.i"
	@echo "... src/mypl_exception.s"
	@echo "... src/print_visitor.o"
	@echo "... src/print_visitor.i"
	@echo "... src/print_visitor.s"
	@echo "... src/semantic_checker.o"
	@echo "... src/semantic_checker.i"
	@echo "... src/semantic_checker.s"
	@echo "... src/simple_parser.o"
	@echo "... src/simple_parser.i"
	@echo "... src/simple_parser.s"
	@echo "... src/symbol_table.o"
	@echo "... src/symbol_table.i"
	@echo "... src/symbol_table.s"
	@echo "... src/token.o"
	@echo "... src/token.i"
	@echo "... src/token.s"
	@echo "... src/var_table.o"
	@echo "... src/var_table.i"
	@echo "... src/var_table.s"
	@echo "... src/vm.o"
	@echo "... src/vm.i"
	@echo "... src/vm.s"
	@echo "... src/vm_instr.o"
	@echo "... src/vm_instr.i"
	@echo "... src/vm_instr.s"
	@echo "... tests/code_generator_tests.o"
	@echo "... tests/code_generator_tests.i"
	@echo "... tests/code_generator_tests.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

