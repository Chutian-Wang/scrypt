# scrypt
> CS32 fall project #1
> scrypt/01 - Calculator.md
> https://github.com/ucsb-cs32/f23/blob/master/scrypt/01%20-%20Calculator.md

## GitHub Rules
- Do not push to the master branch directly. Always create a PR.
- Always run `make clean` before commiting and pushing.
- Code review comments must be meaningful.
- C++ files with main() functions should go directly inside src.
- All other C++ files (including headers) should go inside src/lib.
- Make an announcement to all group members before you edit the Makefile

## Build Instructions
- Make a build folder under the repo folder using `mkdir`.
- Run `make` or `make all` to make, executables are now
generated in the build folder.
- Run `make <module>` to make a specific lib module or
executable.

## Debugging
- GDB/LLDB
- If you are on MacOS, there is a subsitute for Valgrind,
use `leaks --atExit -- <command and args>`. For example,
`leaks --atExit -- ./build/parse < test.txt` will print any
memory leaks that have occured along with other information.
Unfortunately, it usually does not provide you with any
information regarding the source of the leaks. `leaks` comes
with OSX after 2020.

## File Structure
```
scrypt
    |- README.md
    |- Makefile
    |- LICENSE
    |- src
    |   |- main.cpp
    |   |- lib
    |   |   |- lexer.h
    |   |   |- lexer.cpp
    |   |   |- token.h
    |   |   |- AST.h
    |   |   |- AST.cpp
    |   |   |- Nodes.h
    |   |   |- Nodes.cpp
    |   |_  ...
    |
    |- build
    |   |- main
    |   |- lexer.o
    |   |- AST.o
    |_  ...
```

## General Workflow
We work in two thread usually. Both threads fetch from the
master branch for the last checkpoint. Thread A works on the
files and make some commits. When thread A is satisfied with
their work, they submit a PR to be reviewed by thread B. Thread
B will pull from thread A's branch and review the code to make
constructive comments. Thread A will then review thread B's
comments and make changes accordingly and make new commits.
**
When both threads are satisfied, thread B will merge the PR.
**

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
