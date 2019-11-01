#include "test_common.hpp"
#include "test_tree.hpp"

TEST_CASE("test segment tree vanilla") {
    using namespace psds::testing;
    for (int log2 = min_log2; log2 != max_log2 + 1; ++log2) {
        test_tree<segment_tree_vanilla_type>(size_t(1) << log2);
    }
}
