#include "test_common.hpp"
#include "test_tree.hpp"

TEST_CASE("test segment_tree") {
    uint64_t n = sizeof(sizes) / sizeof(sizes[0]);
    for (uint64_t i = 0; i != n; ++i) { test_tree<segment_tree>(sizes[i]); }
}

TEST_CASE("test segment_tree_bottomup") {
    uint64_t n = sizeof(sizes) / sizeof(sizes[0]);
    for (uint64_t i = 0; i != n; ++i) {
        test_tree<segment_tree_bottomup>(sizes[i]);
    }
}
