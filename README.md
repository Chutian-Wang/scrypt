# scrypt
> CS32 fall project #1
> https://github.com/ucsb-cs32/f23/tree/207e30bc6d76e049b77f9fcd1fd22b7cc8aa5863/scrypt

## GitHub Rules
- Do not push to the master branch directly. Always create a PR.
- Code review comments must be meaningful.
- Commit messages must be meaningful.
- Always format your code before submitting for review.
- C++ files with main() functions should go directly inside src.
- All other C++ files (including headers) should go inside src/lib.
- Make an announcement to all group members before you edit the Makefile
- Always update README.md (including the file/class trees) for documentation.

## Build Instructions
- Make a build folder under the repo folder using `mkdir`.
- Run `make` or `make all` to make, executables are now
generated in the build folder.

## Debugging
- GDB/LLDB
- If you are on MacOS, there is a subsitute for Valgrind:
use `leaks --atExit -- <command and args>`. For example,
`leaks --atExit -- ./build/parse < test.txt` will print any
memory leaks that have occured along with other information.
Unfortunately, it sometimes does not provide you with any
information regarding the source of the leaks, e.g. when your
code segfaults. `leaks` comes with OSX after 2020.

## File Structure
```
scrypt
    ├── LICENSE
    ├── Makefile
    ├── README.md
    ├── build
    └── src
        ├── calc.cpp
        ├── format.cpp
        ├── lex.cpp
        ├── lib
        │   ├── AST.cpp
        │   ├── AST.h
        │   ├── Errors.cpp
        │   ├── Errors.h
        │   ├── Lexer.cpp
        │   ├── Lexer.h
        │   ├── Nodes.cpp
        │   ├── Nodes.h
        │   ├── Statement.cpp
        │   ├── Statement.h
        │   ├── Token.cpp
        │   ├── Token.h
        │   ├── Value.cpp
        │   └── Value.h
        ├── parse.cpp
        └── scrypt.cpp
```

## Class Structure and Special Macros
```
Class
    ├── AST (Virtual)
    │   ├── Constant
    │   ├── Operator
    │   └── Identifer
    ├── Statement (Virtual)
    │   ├── Expression
    │   ├── IfStatement
    │   ├── WhileStatement
    │   └── PrintStatement
    ├── std::exception
    │   └── ScryptError (Virtual)
    │       ├── SyntaxError
    │       ├── UnexpTokError
    │       └── ScryptRuntimeError
    │           ├── DivByZero
    │           ├── UnknownIdent
    │           ├── InvalidCond
    │           └── InvalidOperand
    ├── Block (Base)
    ├── Lexer (Base)
    └── Token (Base)

Struct
    └── TokenType (enum)
        ├── NUMBER
        ├── OPERATOR
        ├── LPAREN
        ├── RPAREN
        ├── LCBRACE
        ├── RCBRACE
        ├── IDENTIFIER
        ├── BOOL
        ├── WHILE
        ├── IF
        ├── ELSE
        ├── PRINT
        ├── END
        └── ERR
    
Macros
    └── (Return Codes)
        ├── SYNTAX_ERR      1
        ├── UNEXP_TOK       2
        ├── SCRYPT_RUNTIME  3
        ├── DIV_BY_ZERO     SCRYPT_RUNTIME
        ├── UNKNOWN_IDENT   SCRYPT_RUNTIME
        ├── INVALID_COND    SCRYPT_RUNTIME
        └── INVALID_OPRAND  SCRYPT_RUNTIME
```

## Code Format
We will use the google format conventions for our projects.
To automate the formatting process, you can use clang-format.
clang-format can be installed with most package managing
programs such as apt-get or brew if you are on MacOS.
> https://clang.llvm.org/docs/ClangFormat.html
> Example Usage: `clang-format -i -style=google ./src/*.cpp ./src/lib/*.cpp ./src/lib/*.h`

We will use the following variable naming conventions.
- Functions: camel_case_func()
- Class/Struct/Types: ExampleClass
- Macros: EXAMPLE_MACRO
- ScryptError class naming please refer to error handling section.

## Error Handling
If you checked the class structure above, you would find that we
have a nested error hierachy. This is in place to make automatic
error handling possible. The `ScryptError` base class has a static
function `int ScryptError::handle(std::ostream &cout, const ScryptError &err);`
that will handle any error for you in the catch block without
you needing to worry about error types or return codes, please see the
following example.

```
// this is in main()
try {
    std::cout << parser->eval() << std::endl;
    delete parser;
} catch (const ScryptError &err) {
    // Do not use exit()! use return to avoid hard exit 
    // unwinding problems
    return ScryptError::handle(std::cout, err);
}
```

Please implement any future error classes under the right parent,
e.g. any runtime error should be children of ScriptRuntimeError.

## General Workflow
We work in two threads usually. Both threads fetch from the
master branch for the last checkpoint. Thread A works on the
files and make some commits. When thread A is satisfied with
their work, they submit a PR to be reviewed by thread B. Thread
B will pull from thread A's branch and review the code to make
constructive comments. Thread A will then review thread B's
comments and make changes accordingly and make new commits.
** When both threads are satisfied, thread B will merge the PR. **

## Team Information
|   Name        |   Email                   |
|---------------|---------------------------|
|Barry Wang     |chutian_wang@ucsb.edu      |
|Chloe Ta       |cqta@ucsb.edu              |
|Evania Cheng   |evania@umail.ucsb.edu      |
|Jennie Wu      |Jingyi_wu@umail.ucsb.edu   |

## Class Links
- GitHub for the syllabus, the schedule, and the assignments:
    https://github.com/ucsb-cs32/f23
- Piazza for class announcements and Q&A:
    https://piazza.com/ucsb/fall2023/cs32
- Gradescope for turning in your assignments:
    https://www.gradescope.com/courses/640286

## References
- C++ Reference The simple, user-friendly version:
    https://cplusplus.com/reference/
- C++ Reference The more detailed but less friendly version:
    https://en.cppreference.com/w/
- Bash Cheatsheet For finding your way around the console:
    https://xavierholt.github.io/cheatsheets/bash.html
- Git Cheatsheet For most common Git commands:
    https://xavierholt.github.io/cheatsheets/git.html
- GDB Cheatsheet For when you’re debugging:
    https://xavierholt.github.io/cheatsheets/gdb.html
- Makefile Tutorial To save you a lot of typing:
    https://makefiletutorial.com/
