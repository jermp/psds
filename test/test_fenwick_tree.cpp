#include "test_common.hpp"
#include "test_tree.hpp"

TEST_CASE("test fenwick_tree") {
    uint64_t n = sizeof(sizes) / sizeof(sizes[0]);
    for (uint64_t i = 0; i != n; ++i) test_tree<fenwick_tree>(sizes[i]);
}