mkdir build
mkdir build_no_simd
mkdir build_slow_segtree

cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DSLOW_SEGTREE=OFF -DDISABLE_AVX=OFF -DUSE_SANITIZERS=OFF
make -j
cd ..

cd build_slow_segtree
cmake .. -DCMAKE_BUILD_TYPE=Release -DSLOW_SEGTREE=ON -DDISABLE_AVX=OFF -DUSE_SANITIZERS=OFF
make -j
cd ..

cd build_no_simd
cmake .. -DCMAKE_BUILD_TYPE=Release -DSLOW_SEGTREE=OFF -DDISABLE_AVX=ON -DUSE_SANITIZERS=OFF
make -j
cd ..

cd script
python3 collect_results.py build ../sum.txt ../update.txt
cd ..

# cd build_slow_segtree
# ./perf sts_64u     sum --log segment_tree_simd_node64u_no_opt  2>> ../sum.txt
# ./perf sts_256u    sum --log segment_tree_simd_node256u_no_opt 2>> ../sum.txt
# ./perf sts_64u  update --log segment_tree_simd_node64u_no_opt  2>> ../update.txt
# ./perf sts_256u update --log segment_tree_simd_node256u_no_opt 2>> ../update.txt
# cd ..

cd build_no_simd
./perf sts_64u  update --log segment_tree_node64u  2>> ../update.txt
./perf sts_256u update --log segment_tree_node256u 2>> ../update.txt
./perf sts_64u_restricted  update --log segment_tree_node64u_restricted  2>> ../update.txt
./perf sts_256u_restricted update --log segment_tree_node256u_restricted 2>> ../update.txt
cd ..
