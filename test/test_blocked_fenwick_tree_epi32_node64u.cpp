#include "test_common.hpp"
#include "test_tree.hpp"

TEST_CASE("test blocked_fenwick_tree_epi32_node64u") {
    using namespace psds::testing;
    for (uint32_t log2 = min_log2; log2 != max_log2 + 1; ++log2) {
        test_tree<blocked_fenwick_tree_epi32<node64u>>(size_t(1) << log2);
    }
}