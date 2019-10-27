#include "test_common.hpp"
#include "test_tree.hpp"

TEST_CASE("test tree_epi32_node64u") {
    using namespace psds::testing;

    test_tree<tree_epi32<2, node64u>>(size_t(1) << 8);
    test_tree<tree_epi32<2, node64u>>(size_t(1) << 9);
    test_tree<tree_epi32<2, node64u>>(size_t(1) << 10);
    test_tree<tree_epi32<2, node64u>>(size_t(1) << 11);
    test_tree<tree_epi32<2, node64u>>(size_t(1) << 12);

    test_tree<tree_epi32<3, node64u>>(size_t(1) << 13);
    test_tree<tree_epi32<3, node64u>>(size_t(1) << 14);
    test_tree<tree_epi32<3, node64u>>(size_t(1) << 15);
    test_tree<tree_epi32<3, node64u>>(size_t(1) << 16);
    test_tree<tree_epi32<3, node64u>>(size_t(1) << 17);
    test_tree<tree_epi32<3, node64u>>(size_t(1) << 18);

    test_tree<tree_epi32<4, node64u>>(size_t(1) << 19);
    test_tree<tree_epi32<4, node64u>>(size_t(1) << 20);
    test_tree<tree_epi32<4, node64u>>(size_t(1) << 21);
    test_tree<tree_epi32<4, node64u>>(size_t(1) << 22);
    test_tree<tree_epi32<4, node64u>>(size_t(1) << 23);
    test_tree<tree_epi32<4, node64u>>(size_t(1) << 24);
}
