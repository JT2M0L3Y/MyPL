
cmake_minimum_required(VERSION 3.22)
project(MyPL_2023)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG -Wall -Wextra -Wpedantic")
set(CMAKE_BUILD_TYPE Debug)

include_directories("src")
# include_directories("test")

include(FetchContent)

option(ENABLE_CLANG_TIDY "Run clang-tidy during build" OFF)
if(ENABLE_CLANG_TIDY)
  find_program(CLANG_TIDY clang-tidy)
  if(CLANG_TIDY)
    message(STATUS "clang-tidy found: ${CLANG_TIDY}")
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY};-checks=readability-*,performance-*,portability-*")
  endif()
endif()

# Cppcheck target
find_program(CPPCHECK cppcheck)
if(CPPCHECK)
  message(STATUS "cppcheck found: ${CPPCHECK}")
  add_custom_target(cppcheck
    COMMAND ${CPPCHECK} --std=c++20 --enable=all --suppress=missingIncludeSystem src/
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

# locate gtest
find_package(GTest QUIET)
if(NOT GTest_FOUND)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.13.0.zip
  )
  FetchContent_MakeAvailable(googletest)
endif()

enable_testing()

add_executable(extension_tests tests/extension_tests.cpp
  src/token.cpp src/mypl_exception.cpp src/lexer.cpp src/simple_parser.cpp
  src/ast_parser.cpp src/print_visitor.cpp src/symbol_table.cpp
  src/semantic_checker.cpp src/vm.cpp src/vm_instr.cpp src/var_table.cpp
  src/code_generator.cpp)
if(TARGET GTest::gtest_main)
  target_link_libraries(extension_tests GTest::gtest_main pthread)
else()
  target_link_libraries(extension_tests ${GTEST_LIBRARIES} pthread)
endif()
add_test(NAME extension_tests COMMAND extension_tests)

# create mypl target
add_executable(mypl src/token.cpp src/mypl_exception.cpp src/lexer.cpp
  src/simple_parser.cpp src/ast_parser.cpp src/print_visitor.cpp
  src/symbol_table.cpp src/semantic_checker.cpp src/vm_instr.cpp
  src/vm.cpp src/var_table.cpp src/code_generator.cpp src/mypl.cpp)

