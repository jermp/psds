#include "test_common.hpp"
#include "test_tree.hpp"

TEST_CASE("test fenwick_tree_blocked_node16u") {
    using namespace psds::testing;
    for (uint32_t log2 = min_log2; log2 != max_log2 + 1; ++log2) {
        test_tree<fenwick_tree_blocked<node16u>>(size_t(1) << log2);
    }
}

TEST_CASE("test fenwick_tree_blocked_node64u") {
    using namespace psds::testing;
    for (uint32_t log2 = min_log2; log2 != max_log2 + 1; ++log2) {
        test_tree<fenwick_tree_blocked<node64u>>(size_t(1) << log2);
    }
}

TEST_CASE("test fenwick_tree_blocked_node256u") {
    using namespace psds::testing;
    for (uint32_t log2 = min_log2; log2 != max_log2 + 1; ++log2) {
        test_tree<fenwick_tree_blocked<node256u>>(size_t(1) << log2);
    }
}