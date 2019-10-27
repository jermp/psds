#include "test_common.hpp"
#include "test_tree.hpp"

TEST_CASE("test segment tree") {
    for (int log2 = 8; log2 != 14; ++log2) {
        psds::testing::test_tree<segment_tree_type>(size_t(1) << log2);
    }
}
