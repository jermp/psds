#include "test_common.hpp"
#include "test_tree.hpp"

TEST_CASE("test truncated_fenwick_tree_epi32_node64s") {
    using namespace psds::testing;
    for (uint32_t log2 = min_log2; log2 != max_log2 + 1; ++log2) {
        test_tree<truncated_fenwick_tree_epi32<node64s>>(size_t(1) << log2);
    }
}