#include "test_common.hpp"
#include "test_tree.hpp"

TEST_CASE("test truncated_fenwick_tree_epi32_node256s") {
    using namespace psds::testing;
    test_tree<truncated_fenwick_tree_epi32<node256s>>(size_t(1) << 8);
    test_tree<truncated_fenwick_tree_epi32<node256s>>(size_t(1) << 9);
    test_tree<truncated_fenwick_tree_epi32<node256s>>(size_t(1) << 10);
    test_tree<truncated_fenwick_tree_epi32<node256s>>(size_t(1) << 11);
    test_tree<truncated_fenwick_tree_epi32<node256s>>(size_t(1) << 12);
    test_tree<truncated_fenwick_tree_epi32<node256s>>(size_t(1) << 13);
    test_tree<truncated_fenwick_tree_epi32<node256s>>(size_t(1) << 14);
    test_tree<truncated_fenwick_tree_epi32<node256s>>(size_t(1) << 15);
    test_tree<truncated_fenwick_tree_epi32<node256s>>(size_t(1) << 16);
    // test_tree<truncated_fenwick_tree_epi32<node256s>>(size_t(1) << 17);
    // test_tree<truncated_fenwick_tree_epi32<node256s>>(size_t(1) << 18);
    // test_tree<truncated_fenwick_tree_epi32<node256s>>(size_t(1) << 19);
    // test_tree<truncated_fenwick_tree_epi32<node256s>>(size_t(1) << 20);
    // test_tree<truncated_fenwick_tree_epi32<node256s>>(size_t(1) << 21);
    // test_tree<truncated_fenwick_tree_epi32<node256s>>(size_t(1) << 22);
    // test_tree<truncated_fenwick_tree_epi32<node256s>>(size_t(1) << 23);
    // test_tree<truncated_fenwick_tree_epi32<node256s>>(size_t(1) << 24);
    // test_tree<truncated_fenwick_tree_epi32<node256s>>(size_t(1) << 25);
    // test_tree<truncated_fenwick_tree_epi32<node256s>>(size_t(1) << 26);
    // test_tree<truncated_fenwick_tree_epi32<node256s>>(size_t(1) << 27);
    // test_tree<truncated_fenwick_tree_epi32<node256s>>(size_t(1) << 28);
    // test_tree<truncated_fenwick_tree_epi32<node256s>>(size_t(1) << 29);
    // test_tree<truncated_fenwick_tree_epi32<node256s>>(size_t(1) << 30);
    // test_tree<truncated_fenwick_tree_epi32<node256s>>(size_t(1) << 31);
}