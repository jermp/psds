#include "test_common.hpp"
#include "test_tree.hpp"

TEST_CASE("test fenwick_tree_truncated_node16u") {
    using namespace psds::testing;
    for (uint32_t log2 = min_log2; log2 != max_log2 + 1; ++log2) {
        test_tree<fenwick_tree_truncated<node16u>>(size_t(1) << log2);
    }
}

TEST_CASE("test fenwick_tree_truncated_node64u") {
    using namespace psds::testing;
    for (uint32_t log2 = min_log2; log2 != max_log2 + 1; ++log2) {
        test_tree<fenwick_tree_truncated<node64u>>(size_t(1) << log2);
    }
}

TEST_CASE("test fenwick_tree_truncated_node256u") {
    using namespace psds::testing;
    for (uint32_t log2 = min_log2; log2 != max_log2 + 1; ++log2) {
        test_tree<fenwick_tree_truncated<node256u>>(size_t(1) << log2);
    }
}