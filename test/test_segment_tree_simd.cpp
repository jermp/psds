#include "test_common.hpp"
#include "test_tree.hpp"
#include "util.hpp"

template <int i, template <uint32_t, class> typename Tree, typename Node>
struct test {
    static void run() {
        const uint64_t n = sizes[i];
        const uint32_t height = util::ceil_log(Node::fanout, n);
        typedef typename Tree<height, Node>::tree_type tree_type;
        test_tree<tree_type>(n);
        test<i + 1, Tree, Node>::run();
    }
};

template <template <uint32_t, class> typename Tree, typename Node>
struct test<sizeof(sizes) / sizeof(sizes[0]), Tree, Node> {
    static void run() {}
};

TEST_CASE("test segment_tree_simd_node64u") {
    test<0, segment_tree_simd, node64u>::run();
}

TEST_CASE("test segment_tree_simd_node64u_restricted") {
    test<0, segment_tree_simd, node64u_restricted>::run();
}

TEST_CASE("test segment_tree_simd_node256u") {
    test<0, segment_tree_simd, node256u>::run();
}

TEST_CASE("test segment_tree_simd_node256u_restricted") {
    test<0, segment_tree_simd, node256u_restricted>::run();
}
