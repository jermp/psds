#include "test_common.hpp"
#include "test_tree.hpp"

TEST_CASE("test fenwick_tree_type") {
    for (int log2 = 8; log2 != 24 + 1; ++log2) {
        psds::testing::test_tree<fenwick_tree_type>(size_t(1) << log2);
    }
}