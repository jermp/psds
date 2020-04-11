#include "test_common.hpp"
#include "test_node.hpp"

TEST_CASE("test node64u") {
    test_node<node64u>();
}

TEST_CASE("test node64u_restricted") {
    test_node<node64u_restricted>();
}

TEST_CASE("test node256u") {
    test_node<node256u>();
}

TEST_CASE("test node256u_restricted") {
    test_node<node256u_restricted>();
}