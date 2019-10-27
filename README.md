Prefix-Sum Data Structures
------

Given an array S[0..n), we want to solve
the prefix sums problem.

The library implements the following solutions.

1. Fenwick tree (`fenwick_tree`)
2. Non-recursive Segment tree (`segment_tree_vanilla`)
3. Un-buffered SIMD Segment tree

	Every node has a fanout of 64.
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

4. Buffered SIMD Segment tree

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
	  compute prefix sums on both L and B.


TODO
------

1. Blocked Fenwick trees: blocks of *k* integers are 	   indexed using the Fenwick tree layout.
   During a traversal of the tree, we compute the sum
   or update a block, before proceeding downwards.
   The blocks
   can be stored in prefix-sum (for faster Sum operation)
   or not (for faster Update operation), thus giving
   a similar trade-off already established for the segment
   tree.
   The block factor, i.e., *k*, should be chosen so that
   *k* integers fit in a cache line.
   Assuming a typical cache line size of 64 B and 32-bit
   keys, then *k* = 16.
   Maybe *k* = 8 may be more covenient because
   if we keep the blocks prefix-summed, then
   updating can be done with SIMD using 256-bit registers.
   (See `node64u`).

2. Truncated Fenwick trees: we divide S into blocks
   of size 256. Then the k = ceil(n / 256) blocks
   become the leaves of a fenwick tree.
   Thus we use a traditional (binary? or maybe blocked)
   Fenwick tree truncated as soon as we reduced the range
   down to 256.
   The leaves use the node's implementations already
   developed for the segment trees.
   The nodes of the Fenwick tree will store the prefix
   sums of the blocks.
   Thus we will have two families of algorithms:
   the one based on segment trees; the ones based on
   Fenwick trees.