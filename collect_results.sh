mkdir build
mkdir build_no_simd
mkdir build_slow_segtree

cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DSLOW_SEGTREE=OFF -DDISABLE_AVX=OFF -DUSE_SANITIZERS=OFF
make -j
cd ..

# cd build_slow_segtree
# cmake .. -DCMAKE_BUILD_TYPE=Release -DSLOW_SEGTREE=ON -DDISABLE_AVX=OFF -DUSE_SANITIZERS=OFF
# make -j
# cd ..

# cd build_no_simd
# cmake .. -DCMAKE_BUILD_TYPE=Release -DSLOW_SEGTREE=OFF -DDISABLE_AVX=ON -DUSE_SANITIZERS=OFF
# make -j
# cd ..

cd script
python3 collect_results.py build ../sum_fenwick_tree_family.txt ../update_fenwick_tree_family.txt
cd ..

# cd build_slow_segtree
# ./perf sts_64     sum --log segment_tree_simd_node64_no_opt  2>> ../sum.txt
# ./perf sts_256    sum --log segment_tree_simd_node256_no_opt 2>> ../sum.txt
# ./perf sts_64  update --log segment_tree_simd_node64_no_opt  2>> ../update.txt
# ./perf sts_256 update --log segment_tree_simd_node256_no_opt 2>> ../update.txt
# cd ..

# cd build_no_simd
# ./perf sts_64  update --log segment_tree_node64  2>> ../update.txt
# ./perf sts_256 update --log segment_tree_node256 2>> ../update.txt
# ./perf sts_64_restricted  update --log segment_tree_node64_restricted  2>> ../update.txt
# ./perf sts_256_restricted update --log segment_tree_node256_restricted 2>> ../update.txt
# cd ..
