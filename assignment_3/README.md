# Compiler Construction Framework
The purpose of this framework is to guide implementation of a C-like compiler on x86_64 architectures.


## Modules
Roughly speaking, there exist 4 steps to successfully create a compiler:
 1. Lexical analysis
 2. Syntax analysis
 3. Semantic analysis & intermediate code generation
 4. BinaryNode code generation

In this assignment, we will implement each of these stages.

For each step, we have a module in our framework (found in [`/src/`](/src/)):
 1. [`lexical`](/src/lexical/)
 2. [`syntax`](/src/syntax/)
 3. [`intermediate-code`](/src/intermediate-code/)
 4. [`machine-code`](/src/machine-code/)

Each of these modules generates a library, an executable, and a test executable:
 - The library contains all relevant code of the module. Other modules use the libraries of previous stages. 
   E.g, [`intermediate-code`](/src/intermediate-code/) uses the [`lexical`](/src/lexical/) and [`syntax`](/src/syntax/) libraries.
 - The executable contains a simple commandline interface, so we can play around with our current module and debug it.
 - The test executable launches the testing suite for the current module. Use this executable to verify your implementation.

The executable of the final module ([`machine-code`](/src/machine-code/)) builds the final executable, which is our compiler.

Students are advised to browse through the framework before starting on implementation to familiarize with the framework.
When browsing, start from `/src/<module>/main/cpp/entrypoint/main.cpp`.

### Submodules
Besides the 4 main modules ([`lexical`](/src/lexical/), [`syntax`](/src/syntax/), 
[`intermediate-code`](/src/intermediate-code/), [`machine-code`](/src/machine-code/)),
several other, less important modules exist.
We have:
 1. [`general`](/src/general/): Contains general code, used in almost all projects.
 2. [`syntaxutils`](/src/syntaxutils/): Contains basic data structures for [`lexical`](/src/lexical/) and [`syntax`](/src/syntax/).
 3. [`testutils`](/src/testutils/): Contains code used by all testing code in all modules.

Of these 3 submodules, only [`syntaxutils`](/src/syntaxutils/) should be modified by students.


## Rules
To make debugging and grading much easier, all implementations have to adhere to the following rules:
 1. Do **not** change the framework's source and header files.   
    Of course, all methods containing `TODO: Implement me` comments are of course exempt from this rule.
 
 2. Do **not** add source/header files in any `/src/<module>/src/main/public/`.  
    Reason: We grade your implementations using our own library implementations.
            Student-added signatures are not available in our reference libraries, resulting in errors and a very low grade.

 3. Do **not** hide/override framework implementations.  
    Instead, if you have the feeling something is missing/incorrect, contact us.

 4. Do **not** add more modules.  
    Reason: We grade your implementations using our own library implementations.
            Any extra modules will not be available during our testing, resulting in errors and a very low grade.
 
 5. Do add source and header files if needed.  
    *Don't forget* to add the new source files (`.cpp` files) to Meson. See [docs/MESON.md](/docs/MESON.md) about adding source files.

 6. The implementation must compile using the provided compilation system (see [here](#Meson) for more info).  
    If it does not compile on your machine, it won't compile on our machine either.


## Meson
Meson is a modern, speedy, platform-independent, incremental compiler.
In Meson, we use a python-like syntax to define sources, dependencies, generators etc etc.
In [docs/MESON.md](/docs/MESON.md), we explain how to compile this project with Meson.

**Important**: Your submission must compile with Meson, using the provided Meson files.  
Students are allowed to edit meson files in `/src/<module>/src/main/meson.build`, *but only* to add additional `.cpp` files.


## Submitting issues
This framework is new this year, and bugs/issues will be hidden somewhere.
The original implementers were able to produce a functioning compiler, passing all tests.

If you believe you have found a bug, or require a feature for which you must change 'core' code that must not be changed,
let us know.

## FAQ
Here, we try to answer some frequently asked questions.

1. **Q: Is using Meson mandatory?**   
   A: Yes.

2. **Q: What compiler versions are required and where can I find them on the University machines?**  
   A: This project requires a compiler with full `C++14` support.
      For popular compilers, that means you need _one_ of: 
   + gcc >= 5.2 (version 7.8 default on University computers in `/usr/bin/g++`).
   + clang >= 3.4.
   + llvm >= 6.0.

3. **Q: Does this project work on Windows?**
   A: Not really. On many parts, we just have to assume a Linux distribution is used.
     For example: The assembly we generate uses the Linux syscall for printf.
                  On Windows, there are different procedures to call printf, so your compiled code will not work there, and you cannot test it!
4. **Q: Does this project work on MacOS?**
   A: We do not have much experience with this. Linux and MacOS are pretty similar on many fronts. We assume MacOS is compatible, but no guarantees there.
