15-122 Principles of Imperative Computation
Lab 11: Once you C1 you C them All
​
==========================================================

Files you won't modify:
   lib/contracts.h     - Header for C implementation of contracts
   lib/xalloc.{c,h}    - Library reference of allocating memory in C

Files you will modify:
   ex1.c               - Code for Task 1
                         (Determining then modifying the output of main)
   ex2.c               - Code for Task 2
                         (Fixing Valgrind errors)
   ex3.c               - Code for Task 3
                         (Fixing Valgrind errors by commenting out code)
   ex4.c               - Code for Task 4
                         (Optimizing reverse)

==========================================================

Viewing the documentation for a certain function from string.h:
   % man [FUNCTION NAME]
   % man strcpy

Compiling and running your code for Task 1:
   % gcc -Wall -Wextra -Werror -Wshadow -std=c99 -pedantic -g ex1.c
   % ./a.out
   % valgrind ./a.out

Compiling and running your code for Task 2:
   % gcc -Wall -Wextra -Werror -Wshadow -std=c99 -pedantic -g lib/*.c ex2.c
   % ./a.out
   % valgrind ./a.out

Compiling and running your code for Task 3:
   % gcc -Wall -Wextra -Werror -Wshadow -std=c99 -pedantic -g lib/*.c ex3.c
   % ./a.out
   % valgrind ./a.out

Compiling and running your code for Task 4:
   % gcc -Wall -Wextra -Werror -Wshadow -std=c99 -pedantic -g lib/*.c ex4.c
   % ./a.out
   % valgrind ./a.out
