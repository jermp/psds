#include "test_common.hpp"
#include "test_node.hpp"

TEST_CASE("test node64") {
    test_node<node64>();
}

TEST_CASE("test node64_restricted") {
    test_node<node64_restricted>();
}

TEST_CASE("test node256") {
    test_node<node256>();
}

TEST_CASE("test node256_restricted") {
    test_node<node256_restricted>();
}