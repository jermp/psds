#include "test_common.hpp"
#include "test_node.hpp"

TEST_CASE("test node16u") {
    psds::testing::test_node<node16u>();
}

TEST_CASE("test node64u") {
    psds::testing::test_node<node64u>();
}

TEST_CASE("test node16u") {
    psds::testing::test_node<node256u>();
}

TEST_CASE("test node64s") {
    psds::testing::test_node<node64s>();
}