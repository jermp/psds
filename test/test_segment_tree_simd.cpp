#include "test_common.hpp"
#include "test_tree.hpp"

TEST_CASE("test segment_tree_simd_node64u") {
    using namespace psds::testing;

    test_tree<segment_tree_simd<1, node64u>>(1);
    test_tree<segment_tree_simd<1, node64u>>(10);
    test_tree<segment_tree_simd<1, node64u>>(20);
    test_tree<segment_tree_simd<1, node64u>>(30);
    test_tree<segment_tree_simd<1, node64u>>(40);
    test_tree<segment_tree_simd<1, node64u>>(50);
    test_tree<segment_tree_simd<1, node64u>>(60);

    test_tree<segment_tree_simd<2, node64u>>(70);
    test_tree<segment_tree_simd<2, node64u>>(80);
    test_tree<segment_tree_simd<2, node64u>>(90);
    test_tree<segment_tree_simd<2, node64u>>(100);
    test_tree<segment_tree_simd<2, node64u>>(120);
    test_tree<segment_tree_simd<2, node64u>>(140);
    test_tree<segment_tree_simd<2, node64u>>(160);
    test_tree<segment_tree_simd<2, node64u>>(180);
    test_tree<segment_tree_simd<2, node64u>>(200);
    test_tree<segment_tree_simd<2, node64u>>(220);
    test_tree<segment_tree_simd<2, node64u>>(240);

    test_tree<segment_tree_simd<2, node64u>>(size_t(1) << 8);
    test_tree<segment_tree_simd<2, node64u>>(size_t(1) << 9);
    test_tree<segment_tree_simd<2, node64u>>(size_t(1) << 10);
    test_tree<segment_tree_simd<2, node64u>>(size_t(1) << 11);
    test_tree<segment_tree_simd<2, node64u>>(size_t(1) << 12);

    test_tree<segment_tree_simd<3, node64u>>(size_t(1) << 13);
    test_tree<segment_tree_simd<3, node64u>>(size_t(1) << 14);
    test_tree<segment_tree_simd<3, node64u>>(size_t(1) << 15);
    test_tree<segment_tree_simd<3, node64u>>(size_t(1) << 16);
    test_tree<segment_tree_simd<3, node64u>>(size_t(1) << 17);
    test_tree<segment_tree_simd<3, node64u>>(size_t(1) << 18);

    test_tree<segment_tree_simd<4, node64u>>(size_t(1) << 19);
    test_tree<segment_tree_simd<4, node64u>>(size_t(1) << 20);
    // test_tree<segment_tree_simd<4, node64u>>(size_t(1) << 21);
    // test_tree<segment_tree_simd<4, node64u>>(size_t(1) << 22);
    // test_tree<segment_tree_simd<4, node64u>>(size_t(1) << 23);
    // test_tree<segment_tree_simd<4, node64u>>(size_t(1) << 24);

    // test_tree<segment_tree_simd<5, node64u>>(size_t(1) << 25);
    // test_tree<segment_tree_simd<5, node64u>>(size_t(1) << 26);
    // test_tree<segment_tree_simd<5, node64u>>(size_t(1) << 27);
    // test_tree<segment_tree_simd<5, node64u>>(size_t(1) << 28);
    // test_tree<segment_tree_simd<5, node64u>>(size_t(1) << 29);
    // test_tree<segment_tree_simd<5, node64u>>(size_t(1) << 30);
}

TEST_CASE("test segment_tree_simd_node64u_restricted") {
    using namespace psds::testing;

    test_tree<segment_tree_simd<1, node64u_restricted>>(1);
    test_tree<segment_tree_simd<1, node64u_restricted>>(10);
    test_tree<segment_tree_simd<1, node64u_restricted>>(20);
    test_tree<segment_tree_simd<1, node64u_restricted>>(30);
    test_tree<segment_tree_simd<1, node64u_restricted>>(40);
    test_tree<segment_tree_simd<1, node64u_restricted>>(50);
    test_tree<segment_tree_simd<1, node64u_restricted>>(60);

    test_tree<segment_tree_simd<2, node64u_restricted>>(70);
    test_tree<segment_tree_simd<2, node64u_restricted>>(80);
    test_tree<segment_tree_simd<2, node64u_restricted>>(90);
    test_tree<segment_tree_simd<2, node64u_restricted>>(100);
    test_tree<segment_tree_simd<2, node64u_restricted>>(120);
    test_tree<segment_tree_simd<2, node64u_restricted>>(140);
    test_tree<segment_tree_simd<2, node64u_restricted>>(160);
    test_tree<segment_tree_simd<2, node64u_restricted>>(180);
    test_tree<segment_tree_simd<2, node64u_restricted>>(200);
    test_tree<segment_tree_simd<2, node64u_restricted>>(220);
    test_tree<segment_tree_simd<2, node64u_restricted>>(240);

    test_tree<segment_tree_simd<2, node64u_restricted>>(size_t(1) << 8);
    test_tree<segment_tree_simd<2, node64u_restricted>>(size_t(1) << 9);
    test_tree<segment_tree_simd<2, node64u_restricted>>(size_t(1) << 10);
    test_tree<segment_tree_simd<2, node64u_restricted>>(size_t(1) << 11);
    test_tree<segment_tree_simd<2, node64u_restricted>>(size_t(1) << 12);

    test_tree<segment_tree_simd<3, node64u_restricted>>(size_t(1) << 13);
    test_tree<segment_tree_simd<3, node64u_restricted>>(size_t(1) << 14);
    test_tree<segment_tree_simd<3, node64u_restricted>>(size_t(1) << 15);
    test_tree<segment_tree_simd<3, node64u_restricted>>(size_t(1) << 16);
    test_tree<segment_tree_simd<3, node64u_restricted>>(size_t(1) << 17);
    test_tree<segment_tree_simd<3, node64u_restricted>>(size_t(1) << 18);

    test_tree<segment_tree_simd<4, node64u_restricted>>(size_t(1) << 19);
    test_tree<segment_tree_simd<4, node64u_restricted>>(size_t(1) << 20);
    // test_tree<segment_tree_simd<4, node64u_restricted>>(size_t(1) << 21);
    // test_tree<segment_tree_simd<4, node64u_restricted>>(size_t(1) << 22);
    // test_tree<segment_tree_simd<4, node64u_restricted>>(size_t(1) << 23);
    // test_tree<segment_tree_simd<4, node64u_restricted>>(size_t(1) << 24);

    // test_tree<segment_tree_simd<5, node64u_restricted>>(size_t(1) << 25);
    // test_tree<segment_tree_simd<5, node64u_restricted>>(size_t(1) << 26);
    // test_tree<segment_tree_simd<5, node64u_restricted>>(size_t(1) << 27);
    // test_tree<segment_tree_simd<5, node64u_restricted>>(size_t(1) << 28);
    // test_tree<segment_tree_simd<5, node64u_restricted>>(size_t(1) << 29);
    // test_tree<segment_tree_simd<5, node64u_restricted>>(size_t(1) << 30);
}

TEST_CASE("test segment_tree_simd_node256u") {
    using namespace psds::testing;

    test_tree<segment_tree_simd<1, node256u>>(size_t(1) << 8);

    test_tree<segment_tree_simd<2, node256u>>(size_t(1) << 9);
    test_tree<segment_tree_simd<2, node256u>>(size_t(1) << 10);
    test_tree<segment_tree_simd<2, node256u>>(size_t(1) << 11);
    test_tree<segment_tree_simd<2, node256u>>(size_t(1) << 12);
    test_tree<segment_tree_simd<2, node256u>>(size_t(1) << 13);
    test_tree<segment_tree_simd<2, node256u>>(size_t(1) << 14);
    test_tree<segment_tree_simd<2, node256u>>(size_t(1) << 15);
    test_tree<segment_tree_simd<2, node256u>>(size_t(1) << 16);

    test_tree<segment_tree_simd<3, node256u>>(size_t(1) << 17);
    test_tree<segment_tree_simd<3, node256u>>(size_t(1) << 18);
    test_tree<segment_tree_simd<3, node256u>>(size_t(1) << 19);
    test_tree<segment_tree_simd<3, node256u>>(size_t(1) << 20);
    // test_tree<segment_tree_simd<3, node256u>>(size_t(1) << 21);
    // test_tree<segment_tree_simd<3, node256u>>(size_t(1) << 22);
    // test_tree<segment_tree_simd<3, node256u>>(size_t(1) << 23);
    // test_tree<segment_tree_simd<3, node256u>>(size_t(1) << 24);

    // test_tree<segment_tree_simd<4, node256u>>(size_t(1) << 25);
    // test_tree<segment_tree_simd<4, node256u>>(size_t(1) << 26);
    // test_tree<segment_tree_simd<4, node256u>>(size_t(1) << 27);
    // test_tree<segment_tree_simd<4, node256u>>(size_t(1) << 28);
    // test_tree<segment_tree_simd<4, node256u>>(size_t(1) << 29);
    // test_tree<segment_tree_simd<4, node256u>>(size_t(1) << 30);
}

TEST_CASE("test segment_tree_simd_node256u_restricted") {
    using namespace psds::testing;

    test_tree<segment_tree_simd<1, node256u_restricted>>(size_t(1) << 8);

    test_tree<segment_tree_simd<2, node256u_restricted>>(size_t(1) << 9);
    test_tree<segment_tree_simd<2, node256u_restricted>>(size_t(1) << 10);
    test_tree<segment_tree_simd<2, node256u_restricted>>(size_t(1) << 11);
    test_tree<segment_tree_simd<2, node256u_restricted>>(size_t(1) << 12);
    test_tree<segment_tree_simd<2, node256u_restricted>>(size_t(1) << 13);
    test_tree<segment_tree_simd<2, node256u_restricted>>(size_t(1) << 14);
    test_tree<segment_tree_simd<2, node256u_restricted>>(size_t(1) << 15);
    test_tree<segment_tree_simd<2, node256u_restricted>>(size_t(1) << 16);

    test_tree<segment_tree_simd<3, node256u_restricted>>(size_t(1) << 17);
    test_tree<segment_tree_simd<3, node256u_restricted>>(size_t(1) << 18);
    test_tree<segment_tree_simd<3, node256u_restricted>>(size_t(1) << 19);
    test_tree<segment_tree_simd<3, node256u_restricted>>(size_t(1) << 20);
    // test_tree<segment_tree_simd<3, node256u_restricted>>(size_t(1) << 21);
    // test_tree<segment_tree_simd<3, node256u_restricted>>(size_t(1) << 22);
    // test_tree<segment_tree_simd<3, node256u_restricted>>(size_t(1) << 23);
    // test_tree<segment_tree_simd<3, node256u_restricted>>(size_t(1) << 24);

    // test_tree<segment_tree_simd<4, node256u_restricted>>(size_t(1) << 25);
    // test_tree<segment_tree_simd<4, node256u_restricted>>(size_t(1) << 26);
    // test_tree<segment_tree_simd<4, node256u_restricted>>(size_t(1) << 27);
    // test_tree<segment_tree_simd<4, node256u_restricted>>(size_t(1) << 28);
    // test_tree<segment_tree_simd<4, node256u_restricted>>(size_t(1) << 29);
    // test_tree<segment_tree_simd<4, node256u_restricted>>(size_t(1) << 30);
}
