#include "test_common.hpp"
#include "test_tree.hpp"

TEST_CASE("test fenwick_forest_node64u") {
    using namespace psds::testing;
    static constexpr uint64_t tree_size = 256;

    test_tree<fenwick_forest<1, tree_size, node64u>>(size_t(1) << 8);
    test_tree<fenwick_forest<1, tree_size, node64u>>(size_t(1) << 9);
    test_tree<fenwick_forest<1, tree_size, node64u>>(size_t(1) << 10);
    test_tree<fenwick_forest<1, tree_size, node64u>>(size_t(1) << 11);
    test_tree<fenwick_forest<1, tree_size, node64u>>(size_t(1) << 12);
    test_tree<fenwick_forest<1, tree_size, node64u>>(size_t(1) << 13);
    test_tree<fenwick_forest<1, tree_size, node64u>>(size_t(1) << 14);

    test_tree<fenwick_forest<2, tree_size, node64u>>(size_t(1) << 15);
    test_tree<fenwick_forest<2, tree_size, node64u>>(size_t(1) << 16);
    test_tree<fenwick_forest<2, tree_size, node64u>>(size_t(1) << 17);
    test_tree<fenwick_forest<2, tree_size, node64u>>(size_t(1) << 18);
    test_tree<fenwick_forest<2, tree_size, node64u>>(size_t(1) << 19);
    test_tree<fenwick_forest<2, tree_size, node64u>>(size_t(1) << 20);

    test_tree<fenwick_forest<3, tree_size, node64u>>(size_t(1) << 21);
    test_tree<fenwick_forest<3, tree_size, node64u>>(size_t(1) << 22);
    test_tree<fenwick_forest<3, tree_size, node64u>>(size_t(1) << 23);
    test_tree<fenwick_forest<3, tree_size, node64u>>(size_t(1) << 24);
}

TEST_CASE("test fenwick_forest_node256u") {
    using namespace psds::testing;
    static constexpr uint64_t tree_size = 256;

    test_tree<fenwick_forest<1, tree_size, node256u>>(size_t(1) << 8);
    test_tree<fenwick_forest<1, tree_size, node256u>>(size_t(1) << 9);
    test_tree<fenwick_forest<1, tree_size, node256u>>(size_t(1) << 10);
    test_tree<fenwick_forest<1, tree_size, node256u>>(size_t(1) << 11);
    test_tree<fenwick_forest<1, tree_size, node256u>>(size_t(1) << 12);
    test_tree<fenwick_forest<1, tree_size, node256u>>(size_t(1) << 13);
    test_tree<fenwick_forest<1, tree_size, node256u>>(size_t(1) << 14);
    test_tree<fenwick_forest<1, tree_size, node256u>>(size_t(1) << 15);
    test_tree<fenwick_forest<1, tree_size, node256u>>(size_t(1) << 16);

    test_tree<fenwick_forest<2, tree_size, node256u>>(size_t(1) << 17);
    test_tree<fenwick_forest<2, tree_size, node256u>>(size_t(1) << 18);
    test_tree<fenwick_forest<2, tree_size, node256u>>(size_t(1) << 19);
    test_tree<fenwick_forest<2, tree_size, node256u>>(size_t(1) << 20);
    test_tree<fenwick_forest<2, tree_size, node256u>>(size_t(1) << 21);
    test_tree<fenwick_forest<2, tree_size, node256u>>(size_t(1) << 22);
    test_tree<fenwick_forest<2, tree_size, node256u>>(size_t(1) << 23);
    test_tree<fenwick_forest<2, tree_size, node256u>>(size_t(1) << 24);
}
