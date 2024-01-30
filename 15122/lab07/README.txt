15-122 Principles of Imperative Computation
Lab 07: List(en) Up!

==========================================================

Files you won't modify:
   lib/listlib.c0       - Declarations of list types, along with various
                          specification and helper functions
   sortedlist-bad1.c0   - Hyrum's broken implementation 1
   sortedlist-bad2.c0   - Hyrum's broken implementation 2
   sortedlist-bad3.c0   - Hyrum's broken implementation 3
   sortedlist-bad4.c0   - Hyrum's broken implementation 4
   sortedlist-bad5.c0   - Hyrum's broken implementation 5
   sortedlist.c0        - File of empty implementations
                          (only used to check if your test cases compile)

Files you will modify:
   sortedlist-test.c1   - Add your test cases here

==========================================================

If you wish to check if your code compiles, you can run
   % cc0 listlib.c0 sortedlist.c0 sortedlist-test.c0

You can compile and run your code with these commands (one for each bad)
   % make 1
   % make 2
   % make 3
   % make 4
   % make 5

If you don't want to use the makefile, here can alternatively use
   % cc0 -x listlib.c0 sortedlist-bad1.c0 sortedlist-test.c0
   % cc0 -x listlib.c0 sortedlist-bad2.c0 sortedlist-test.c0
   % cc0 -x listlib.c0 sortedlist-bad3.c0 sortedlist-test.c0
   % cc0 -x listlib.c0 sortedlist-bad4.c0 sortedlist-test.c0
   % cc0 -x listlib.c0 sortedlist-bad5.c0 sortedlist-test.c0
