Prefix-Sum Data Structures
==========================

Given an integer array A[0..n), the *prefix-sum problem*
asks for a data structure built from A that supports
for any 0 ≤ i < n:

- sum(i) = A[0] + ... + A[i];
- update(i, x) which sets A[i] to A[i] + x.
- access(i) = A[i].

The library implements the following solutions to solve the problem.

- binary Segment-Tree (top-down and bottom-up)
- b-ary Segment-Tree (b > 2)
- Fenwick-Tree
- b-ary Fenwick-Tree
- blocked Fenwick-Tree with blocks of b keys
- truncated Fenwick-Tree with blocks of leaves of b keys

For a description and anlysis of all these data structures,
see the paper [Practical Trade-Offs for the Prefix-Sum Problem](https://arxiv.org/pdf/2006.14552.pdf), by Giulio Ermanno Pibiri and Rossano Venturini.

Compiling the code <a name="compiling"></a>
------------------

The code is tested on Linux with `gcc` 7.4 and 9.2.1; on Mac 10.14 with `clang` 10.0.0 and 11.0.0.
To build the code, [`CMake`](https://cmake.org/) is required.

Clone the repository with

	git clone --recursive https://github.com/jermp/psds.git

If you have cloned the repository without `--recursive`, you will need to perform the following commands before
compiling:

    git submodule init
    git submodule update

To compile the code for a release environment (see file `CMakeLists.txt` for the used compilation flags), it is sufficient to do the following:

    mkdir build
    cd build
    cmake ..
    make -j

By default, SIMD AVX instructions are enabled (flag `-DDISABLE_AVX=Off`). If you want to
disable them (although your compiler has proper support), you can compile with

	cmake .. -DDISABLE_AVX=On
	make -j


For the best of performance, we recommend compiling with (default configuration):

	cmake .. -DCMAKE_BUILD_TYPE=Release -DUSE_SANITIZERS=Off -DDISABLE_AVX=Off
	make -j

For a testing environment, use the following instead:

    mkdir debug_build
    cd debug_build
    cmake .. -DCMAKE_BUILD_TYPE=Debug -DUSE_SANITIZERS=On
    make -j

Benchmarks
---------

To benchmark the running time of sum and update for the disired data structure, use the program `src/perf`. Running the program
without arguments will show what arguments are required.
(See also the file `src/perf.cpp` for a list of available
data structure types.)

Below we show some examples.


- The command

		./perf ft sum

will benchmark the speed of sum queries for the Fenwick Tree data structure (`ft`), by varying the size of the input array from 250 to 1,000,000,000.

- The command

		./perf sts_256 update -i 40

will benchmark the speed of updates for the SIMD Segment Tree
with branching factor 256 (`sts_256`) for an array
of size floor((1.25893)^40) = 2,511,886.

Unit tests <a name="testing"></a>
-----------

The unit tests are written using [doctest](https://github.com/onqtam/doctest).

After compilation, it is advised
to run the unit tests with:

	make test
