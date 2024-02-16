# README

<a name="title"></a>
## MyPL - Programming Language Interpreter

<a name="desc"></a>
### Description
This programming language is a work in progress. I finished a basic set of operations, constructs, and methods that can be performed as per the requirements of a PL course. Beyond this, I have started extending this language by adding an additional construct: dictionaries (Python) or undordered maps (C++). 

<a name="tbl"></a>
### Table of Contents
- [Install](#install)
- [Usage](#usage)
- [Credits](#cred)

<a name="install"></a>
### Installation

To "install" this language one must run a set of commands:
- CMake to build tools that will process dependencies
```
cmake .
```
- Make to process dependencies and build the language
```
make
```

<a name="usage"></a>
### Usage

To run:
```
./mypl [flag] [file]
```

Available flags:  
`--lex`: lexical analysis  
`--parse`: parsing, AST generation  
`--print`: pretty printer  
`--check`: semantic checking  
`--ir`: machine instruction generation  
`--help`: usage message  

Features:
- lexical analysis
- simple parsing
- AST generation
- pretty printer
- semantic checking
- machine instruction generation with VM
- running programs written in 'MyPL'

Language Benefits:
- types
  - user-defined ```struct [name] { [fields]* }```
  - 1D arrays ```array [type] [id] = new [type][ [size] ]```
  - primitive ```[type] [id] = [val]```
- methods
  - length (string, arrays) ```length([id])```
  - get/set fields ```[id][val]```
  - get/set array index ```[id][val]```
  - get char in string ```get(val)```
  - terminal I/O (print, input) ```print([type]), input()```
- extended use
  - dictionary (unordered map) type ```dict [type1] [type2] [id] = new dict{type1, type2}```
  - methods: keys, values ```keys([id]), values([id])```

An example of this language's use has been published to 
[YouTube](https://www.youtube.com/watch?v=ngUi5oREe8E).

<a name="cred"></a>
### Credits

Contributors:
- JT2M0L3Y
- Dr. Bowers
