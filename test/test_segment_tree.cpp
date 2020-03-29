#include "test_common.hpp"
#include "test_tree.hpp"

TEST_CASE("test segment_tree") {
    using namespace psds::testing;
    for (uint32_t log2 = min_log2; log2 != max_log2 + 1; ++log2) {
        test_tree<segment_tree>(size_t(1) << log2);
    }
}
