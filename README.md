## Advent of Code 2016

During 2017, I had chosen [Advent Of Code 2016](http://adventofcode.com/2016) to have some fun by coding.

Also, I had set some personal rules: 
1. every code should be written (mostly) in imperative style;
2. every code should be written in (modern) C++ or C;
3. I could you use only the [Standard Template Library](https://en.wikipedia.org/wiki/Standard_Template_Library) (STL) as facility, no other C or C++ library.

So, right now I've decided to share my experience in this repository. Enjoy! 

![Advent Of Code 2016  completed!](https://github.com/fabiogaluppo/AdventOfCode2016/raw/master/images/AdventOfCode2016.png)

Compile with:
* clang
  * **clang -O3 -c md5c.c -o ./bin/md5c.o**
  * **clang++ -std=c++14 -O3 -Wno-write-strings ./bin/md5c.o program.cpp -o ./bin/AdventOfCode2016.exe**

* msvc
  * **cl.exe /c /Fo.\bin\ /Ox md5c.c**
  * **cl.exe /Fo.\bin\ /std:c++14 /EHsc /Ox .\bin\md5c.obj program.cpp /link /out:.\bin\AdventOfCode2016.exe**

* gcc
  * **gcc -O3 -c md5c.c -o ./bin/md5c.o**
  * **g++ -std=c++14 -O3 -Wno-write-strings ./bin/md5c.o program.cpp -o ./bin/AdventOfCode2016.exe**

##### Compile and Run with gcc
![Compile and Run with gcc](https://github.com/fabiogaluppo/AdventOfCode2016/raw/master/images/AdventOfCode2016-compile-and-run-gcc.png)

##### Compile and Run with msvc
![Compile and Run with msvc](https://github.com/fabiogaluppo/AdventOfCode2016/raw/master/images/AdventOfCode2016-compile-and-run-msvc.png)