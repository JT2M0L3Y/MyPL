# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

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
CMAKE_SOURCE_DIR = "/mnt/c/Users/jtsmo/OneDrive - Gonzaga University/junior/spring/opl/project-JT2M0L3Y"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/c/Users/jtsmo/OneDrive - Gonzaga University/junior/spring/opl/project-JT2M0L3Y"

# Include any dependencies generated for this target.
include CMakeFiles/token_tests.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/token_tests.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/token_tests.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/token_tests.dir/flags.make

CMakeFiles/token_tests.dir/tests/token_tests.cpp.o: CMakeFiles/token_tests.dir/flags.make
CMakeFiles/token_tests.dir/tests/token_tests.cpp.o: tests/token_tests.cpp
CMakeFiles/token_tests.dir/tests/token_tests.cpp.o: CMakeFiles/token_tests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/jtsmo/OneDrive - Gonzaga University/junior/spring/opl/project-JT2M0L3Y/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/token_tests.dir/tests/token_tests.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/token_tests.dir/tests/token_tests.cpp.o -MF CMakeFiles/token_tests.dir/tests/token_tests.cpp.o.d -o CMakeFiles/token_tests.dir/tests/token_tests.cpp.o -c "/mnt/c/Users/jtsmo/OneDrive - Gonzaga University/junior/spring/opl/project-JT2M0L3Y/tests/token_tests.cpp"

CMakeFiles/token_tests.dir/tests/token_tests.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/token_tests.dir/tests/token_tests.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Users/jtsmo/OneDrive - Gonzaga University/junior/spring/opl/project-JT2M0L3Y/tests/token_tests.cpp" > CMakeFiles/token_tests.dir/tests/token_tests.cpp.i

CMakeFiles/token_tests.dir/tests/token_tests.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/token_tests.dir/tests/token_tests.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Users/jtsmo/OneDrive - Gonzaga University/junior/spring/opl/project-JT2M0L3Y/tests/token_tests.cpp" -o CMakeFiles/token_tests.dir/tests/token_tests.cpp.s

CMakeFiles/token_tests.dir/src/token.cpp.o: CMakeFiles/token_tests.dir/flags.make
CMakeFiles/token_tests.dir/src/token.cpp.o: src/token.cpp
CMakeFiles/token_tests.dir/src/token.cpp.o: CMakeFiles/token_tests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/jtsmo/OneDrive - Gonzaga University/junior/spring/opl/project-JT2M0L3Y/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/token_tests.dir/src/token.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/token_tests.dir/src/token.cpp.o -MF CMakeFiles/token_tests.dir/src/token.cpp.o.d -o CMakeFiles/token_tests.dir/src/token.cpp.o -c "/mnt/c/Users/jtsmo/OneDrive - Gonzaga University/junior/spring/opl/project-JT2M0L3Y/src/token.cpp"

CMakeFiles/token_tests.dir/src/token.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/token_tests.dir/src/token.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Users/jtsmo/OneDrive - Gonzaga University/junior/spring/opl/project-JT2M0L3Y/src/token.cpp" > CMakeFiles/token_tests.dir/src/token.cpp.i

CMakeFiles/token_tests.dir/src/token.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/token_tests.dir/src/token.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Users/jtsmo/OneDrive - Gonzaga University/junior/spring/opl/project-JT2M0L3Y/src/token.cpp" -o CMakeFiles/token_tests.dir/src/token.cpp.s

# Object files for target token_tests
token_tests_OBJECTS = \
"CMakeFiles/token_tests.dir/tests/token_tests.cpp.o" \
"CMakeFiles/token_tests.dir/src/token.cpp.o"

# External object files for target token_tests
token_tests_EXTERNAL_OBJECTS =

token_tests: CMakeFiles/token_tests.dir/tests/token_tests.cpp.o
token_tests: CMakeFiles/token_tests.dir/src/token.cpp.o
token_tests: CMakeFiles/token_tests.dir/build.make
token_tests: /usr/lib/x86_64-linux-gnu/libgtest.a
token_tests: CMakeFiles/token_tests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/c/Users/jtsmo/OneDrive - Gonzaga University/junior/spring/opl/project-JT2M0L3Y/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable token_tests"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/token_tests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/token_tests.dir/build: token_tests
.PHONY : CMakeFiles/token_tests.dir/build

CMakeFiles/token_tests.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/token_tests.dir/cmake_clean.cmake
.PHONY : CMakeFiles/token_tests.dir/clean

CMakeFiles/token_tests.dir/depend:
	cd "/mnt/c/Users/jtsmo/OneDrive - Gonzaga University/junior/spring/opl/project-JT2M0L3Y" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Users/jtsmo/OneDrive - Gonzaga University/junior/spring/opl/project-JT2M0L3Y" "/mnt/c/Users/jtsmo/OneDrive - Gonzaga University/junior/spring/opl/project-JT2M0L3Y" "/mnt/c/Users/jtsmo/OneDrive - Gonzaga University/junior/spring/opl/project-JT2M0L3Y" "/mnt/c/Users/jtsmo/OneDrive - Gonzaga University/junior/spring/opl/project-JT2M0L3Y" "/mnt/c/Users/jtsmo/OneDrive - Gonzaga University/junior/spring/opl/project-JT2M0L3Y/CMakeFiles/token_tests.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/token_tests.dir/depend

