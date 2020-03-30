Prefix-Sum Data Structures
------

Given an array S[0..n), we want to solve
the prefix-sum problem.

The library implements the following solutions.

- Binary Segment Tree

- Fenwick Tree
 
- Un-buffered *k*-ary Segment Tree with SIMD

<!--	Every node has a fanout of 64.
	A block S[0..63] of 64 integers is divided into 8
	segments
	of 8 integers each. For each segment we store
	a summary information in an array U[0..7+1]:
	U[i + 1] = U[i] + *sum*,
	for i = 0..7, where *sum* is the sum of the 8
	integers in the i-th segment and U[0] = 0.

	Thus, every node is a 2-level data structure.

	We implemented two versions of the node.

	- Fast Sum (`node64u`).
	  We store each segment in a prefix-sum
	  fashion.

	  Sum(i) queries are as easy as:
	  U[i / 8] + S[i].

	  During Update we use SIMD
	  (with a look-up table)
	  to sum in parallel 8 integers to both U and
	  a specific segment of S.

	- Fast Update (`node64s`).
	  We *do not* store segments using
	  prefix-sums.

	  Therefore updates just need to modify U[i / 8] and
	  S[i % 8].

	  We use SIMD during the Sum query to
	  compute the prefix sum of a segment of 8 integers.

3. ##### Buffered SIMD *k*-ary Segment tree

	Every node has a fanout of 256.
	A block S[0..255] of 256 integers is divided into 8
	segments
	of 32 integers each. We use an array B[0..255]
	of 256 X 8-bit integers to buffer the updates
	for S.

	Every node is a 2-level data structure, as explained
	below.

	- Fast Sum (`node256u`).

	  We store each segment of 32 integers in a
	  prefix-sum fashion and keep block-level
	  prefix-sums in an array L[0..7+1]:
	  L[i + 1] = L[i] + *sum*,
	  for i = 0..7, where *sum* is the sum of the 32
	  integers in the i-th segment and L[0] = 0.

	  Sum(i) queries are as easy as:
	  L[i / 32] + B[i] + S[i].

	  During Update we use SIMD
	  (with a look-up table)
	  to sum in parallel integers to both L and B.
	  To resolve overflow issues, we count
	  the number of updates with a *counter* variable.
	  When *counter* = 127 (worst-case bound)
	  we sum B and S, reset B and set *counter* = 0.

	- Fast Update (`node256s`).
	  We store each segment in a prefix-sum
	  fashion and use another buffer L[0..7] of
	  8 X 16-bit integers to keep track of the
	  updates in a segment of 32 integers.

	  Therefore updates just need to modify L[i / 32] and
	  B[i]. When B[i] stores the maximum (or minimum)
	  signed 8-bit integer, we sum B and S and reset both
	  B and L.

	  We use SIMD during the Sum query to
	  compute prefix sums on both L and B.-->
	  
- Truncated Fenwick Tree

<!--	We divide S into blocks of size *k*. Then the ceil(*n* / *k*) blocks become the leaves of a fenwick tree. Thus we use a Fenwick tree truncated as soon as we reduced the range down to *k*. The nodes of the Fenwick tree will store the prefix sums of the blocks.-->

- Blocked Fenwick Tree

<!--	Blocks of *k* integers are
   indexed using the Fenwick tree layout.
   During a traversal of the tree, we compute the sum
   or update a block, before proceeding downwards.
   The blocks
   can be stored in prefix-sum (for faster Sum operation)
   or not (for faster Update operation), thus giving
   a similar trade-off already established for the segment
   tree.
   The block size *k* should be chosen so that
   *k* integers fit in a cache line.
   Assuming a typical cache line size of 64 B and 32-bit
   keys, then *k* = 16.
   Maybe *k* = 8 may be more covenient because
   if we keep the blocks prefix-summed, then
   updating can be done with SIMD using 256-bit registers.
   (See `node64u`).-->

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
	make


For the best of performance, we recommend compiling with (default configuration):

	cmake .. -DCMAKE_BUILD_TYPE=Release -DUSE_SANITIZERS=Off -DDISABLE_AVX=Off
	make -j
	
For a testing environment, use the following instead:

    mkdir debug_build
    cd debug_build
    cmake .. -DCMAKE_BUILD_TYPE=Debug -DUSE_SANITIZERS=On
    make -j

Running the unit tests <a name="testing"></a>
-----------

The unit tests are written using [doctest](https://github.com/onqtam/doctest).

After compilation, it is advised
to run the unit tests with:

	make test

Results
------

### <span style="color:red">Warning: These considerations are relative to a previous implementation. Should be revised accordingly.</span>

1. The binary segment tree is always outperformed by both the (binary) fenwick tree
   and the non-binary SIMD-ized segment trees (`tree_epi32`).
   This is due to its poor cache exploitation given by the large tree height. The Fenwick tree is faster then a binary segment tree because
   it is smaller (it is an implicit data structure taking *n* space, whereas the segment tree takes at least the double) and its average
   cost is better.

2. After enlarging the degree of a node, SIMD instructions are very effective to improve the running time of the segment tree. Without AVX there is no appreciable difference between a traditional segment tree and the blocked segment trees.
With AVX, sum becomes ~2-4X faster; update ~8X faster.

3. SIMD instructions are more useful when executing updates rather than prefix sums.
   This is valid for both buffered (`*node256*`) and un-buffered (`*node64*`) segment trees.
   This is evident by considering that the solution `tree_epi32_node256s` is not faster than the fenwick tree
   (or even worse for larger values of *n*);
   but the solution `tree_epi32_node256u` is consistently ~2X faster than the fenwick
   tree on updates for all values of *n*. See also the plots for the values of *n*.
   (The `tree_epi32_node256s` is ~4X faster than the
   fenwick tree, though.)
   This is because the code for computing prefix-sums using SIMD is more complicated
   and uses higher-latency instructions than the code performing update.

4. Results for buffered and un-buffered segment trees are similar. The un-buffered
   version can be slightly faster on some small values of *n* (`tree_epi32_node64u` is faster
   than `tree_epi32_node256u` for log *n* = 9, 10, 11 on sum and
   for log *n* = 9, 10, 11, 12 on update);
   the buffered version is generally better on larger values of *n*.

5. Blocked Fenwick trees are more efficient than traditional Fenwick trees because the height is reduced from log(*n*) to log(*n*/B).
The *average* number of nodes traversed will then be log(*n*/B)/2 for
uniformly distributed (random) queries.

6. However, Blocked Fenwick trees perform worse than a segment tree with the same block arity B in the worst case. This is because the segment tree will always (in the worst case) traverse log(*n*)/log(B) nodes,
against log(*n*/B) nodes.

7. Truncated Fenwick trees shine for larger values of *n* because the "high" part of the data structure, that is a Fenwick tree, is likely of fit in cache without using any space overhead, and we only apply SIMD on the identified leaf. The segment tree using the same node structure performs worse because every node uses extra space, causing the tree to exit the cache before the truncated variant. Thus the truncated variant wins out because of the reduced number of cache misses.
