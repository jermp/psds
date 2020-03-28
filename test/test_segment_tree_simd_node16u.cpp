#include "test_common.hpp"
#include "test_tree.hpp"

TEST_CASE("test segment_tree_simd_node16u") {
    using namespace psds::testing;

    test_tree<segment_tree_simd<2, node16u>>(size_t(1) << 8);

    test_tree<segment_tree_simd<3, node16u>>(size_t(1) << 9);
    test_tree<segment_tree_simd<3, node16u>>(size_t(1) << 10);
    test_tree<segment_tree_simd<3, node16u>>(size_t(1) << 11);
    test_tree<segment_tree_simd<3, node16u>>(size_t(1) << 12);

    test_tree<segment_tree_simd<4, node16u>>(size_t(1) << 13);
    test_tree<segment_tree_simd<4, node16u>>(size_t(1) << 14);
    test_tree<segment_tree_simd<4, node16u>>(size_t(1) << 15);
    test_tree<segment_tree_simd<4, node16u>>(size_t(1) << 16);
}