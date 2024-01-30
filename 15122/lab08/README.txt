15-122 Principles of Imperative Computation
Lab 08: Hash This!

==========================================================

Files you won't modify:
   texts/dictionary.txt - Scrabble dictionary
   texts/profs.txt      - List of CS faculty info
   hash-dictionary.c0   - Hashes texts/dictionary.txt
   visualizer.c0        - Visualizes the number of items hashed to
                          each bucket. Default bucket size is 512

Files you will modify:
   my-hash-mul32.c0     - Implement hash_string. Do not change the
                          name of the functions
   my-hash-lcg.c0       - Implement hash_string. Do not change the
                          name of the functions
   hash-profs.c0        - Implement hash_prof.  Do not change other
                          parts.  Hashes tedts/profs.txt.

==========================================================

Running reference hash functions:
   % hash_add
   % hash_mul31
   % hash_mul32
   % hash_lcg
Note: Enter an empty line to quit

Testing your hash_mul32 function:
   % coin -d my-hash-mul32.c0
   --> hash_string("some strings of your choice");
   and compare with the output of hash_mul32 on those strings

Testing your hash_lcg function:
   % coin -d my-hash-lcg.c0
   --> hash_string("some strings of your choice");
   and compare with the output of hash_lcg on those strings

Compiling and running your hash_mul32 function:
   % cc0 -o my-hash-mul32 my-hash-mul32.c0 hash-dictionary.c0 visualizer.c0
   % ./my_hash_mul32 -o my-mul32.png
   % display my-mul32.png

Compiling and running your hash_lcg function:
   % cc0 -o my-hash-lcg my-hash-lcg.c0 hash-dictionary.c0 visualizer.c0
   % ./my-hash-lcg -o my-lcg.png
   % display my-lcg.png

Compiling and running your professor hashing function with your hash_mul32:
   % cc0 -o profs-mul32 my-hash-mul32.c0 hash-profs.c0 visualizer.c0
   % ./profs-mul32 -o profs-mul32.png
   % display profs-mul32.png

Compiling and running your professor hashing function your hash_lcg:
   % cc0 -o profs-lcg my-hash-lcg.c0 hash-profs.c0 visualizer.c0
   % ./profs-lcg -o profs-lcg.png
   % display profs-lcg.png

Note: Be sure to use -X or -Y when ssh'ing
Note: Use -n <desired table size> on executable to change
      bucket size from default of 512. May be useful when
      hashing profs.
