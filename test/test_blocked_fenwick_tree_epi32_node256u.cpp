#include "test_common.hpp"
#include "test_tree.hpp"

// struct node4 {
//     static constexpr uint32_t degree = 4;
//     static constexpr uint32_t size = 4 * 4;

//     static void build(int32_t const* input, uint8_t* out) {
//         std::fill(out, out + size, 0);
//         int32_t* S = reinterpret_cast<int32_t*>(out);
//         for (uint32_t i = 0; i != degree; ++i) S[i] = input[i];
//     }

//     static std::string name() {
//         return "node4";
//     }

//     node4(uint8_t* ptr) {
//         S = reinterpret_cast<int32_t*>(ptr);
//     }

//     void update(uint32_t i, int8_t delta) {
//         assert(i < degree);
//         assert(delta == +1 or delta == -1);
//         S[i] += delta;
//     }

//     int32_t sum(uint32_t i) const {
//         assert(i < degree);
//         int32_t s = 0;
//         for (uint32_t k = 0; k <= i; ++k) s += S[k];
//         return s;
//     }

// private:
//     int32_t* S;
// };

TEST_CASE("test blocked_fenwick_tree_epi32_node256u") {
    using namespace psds::testing;

    // std::vector<int32_t> vec = {3, 2, -1, 0, 2, 2,  1, -2,
    //                             3, 5, 1,  2, 1, -2, 3, 7};
    // blocked_fenwick_tree_epi32<node4> tree;
    // tree.build(vec.data(), vec.size());

    // essentials::logger("testing sum queries...");
    // int32_t expected = 0;
    // for (uint32_t i = 0; i != vec.size(); ++i) {
    //     int32_t got = tree.sum(i);
    //     expected += vec[i];
    //     CHECK_MESSAGE(got == expected, "got sum(" << i << ") = " << got
    //                                               << " but expected "
    //                                               << expected);
    // }

    // auto update = [&](int8_t delta) {
    //     essentials::logger("testing update " + std::to_string(int(delta)) +
    //                        " queries...");
    //     for (uint32_t run = 0; run != 256; ++run) {
    //         int32_t expected = 0;
    //         for (uint32_t i = 0; i != vec.size(); ++i) {
    //             tree.update(i, delta);
    //             int32_t got = tree.sum(i);
    //             vec[i] += delta;
    //             expected += vec[i];
    //             REQUIRE_MESSAGE(got == expected,
    //                             "error during run "
    //                                 << run << ": got sum(" << i << ") = " <<
    //                                 got
    //                                 << " but expected " << expected);
    //         }
    //     }
    // };

    // update(+1);
    // update(-1);

    // test_tree<blocked_fenwick_tree_epi32<node4>>(size_t(1) << 8);
    // test_tree<blocked_fenwick_tree_epi32<node4>>(size_t(1) << 9);
    // test_tree<blocked_fenwick_tree_epi32<node4>>(size_t(1) << 10);
    // test_tree<blocked_fenwick_tree_epi32<node4>>(size_t(1) << 11);
    // test_tree<blocked_fenwick_tree_epi32<node4>>(size_t(1) << 12);
    // test_tree<blocked_fenwick_tree_epi32<node4>>(size_t(1) << 13);
    // test_tree<blocked_fenwick_tree_epi32<node4>>(size_t(1) << 14);
    // test_tree<blocked_fenwick_tree_epi32<node4>>(size_t(1) << 15);
    // test_tree<blocked_fenwick_tree_epi32<node4>>(size_t(1) << 16);
    // test_tree<blocked_fenwick_tree_epi32<node4>>(size_t(1) << 17);
    // test_tree<blocked_fenwick_tree_epi32<node4>>(size_t(1) << 18);
    // test_tree<blocked_fenwick_tree_epi32<node4>>(size_t(1) << 19);
    // test_tree<blocked_fenwick_tree_epi32<node4>>(size_t(1) << 20);

    test_tree<blocked_fenwick_tree_epi32<node256u>>(size_t(1) << 8);
    test_tree<blocked_fenwick_tree_epi32<node256u>>(size_t(1) << 9);
    test_tree<blocked_fenwick_tree_epi32<node256u>>(size_t(1) << 10);
    test_tree<blocked_fenwick_tree_epi32<node256u>>(size_t(1) << 11);
    test_tree<blocked_fenwick_tree_epi32<node256u>>(size_t(1) << 12);
    test_tree<blocked_fenwick_tree_epi32<node256u>>(size_t(1) << 13);
    test_tree<blocked_fenwick_tree_epi32<node256u>>(size_t(1) << 14);
    test_tree<blocked_fenwick_tree_epi32<node256u>>(size_t(1) << 15);
    test_tree<blocked_fenwick_tree_epi32<node256u>>(size_t(1) << 16);
    // test_tree<blocked_fenwick_tree_epi32<node256u>>(size_t(1) << 17);
    // test_tree<blocked_fenwick_tree_epi32<node256u>>(size_t(1) << 18);
    // test_tree<blocked_fenwick_tree_epi32<node256u>>(size_t(1) << 19);
    // test_tree<blocked_fenwick_tree_epi32<node256u>>(size_t(1) << 20);
    // test_tree<blocked_fenwick_tree_epi32<node256u>>(size_t(1) << 21);
    // test_tree<blocked_fenwick_tree_epi32<node256u>>(size_t(1) << 22);
    // test_tree<blocked_fenwick_tree_epi32<node256u>>(size_t(1) << 23);
    // test_tree<blocked_fenwick_tree_epi32<node256u>>(size_t(1) << 24);

    // test_tree<blocked_fenwick_tree_epi32<node4>>(size_t(1) << 25);
    // test_tree<blocked_fenwick_tree_epi32<node4>>(size_t(1) << 26);
    // test_tree<blocked_fenwick_tree_epi32<node4>>(size_t(1) << 27);
    // test_tree<blocked_fenwick_tree_epi32<node4>>(size_t(1) << 28);
    // test_tree<blocked_fenwick_tree_epi32<node4>>(size_t(1) << 29);
    // test_tree<blocked_fenwick_tree_epi32<node4>>(size_t(1) << 30);
    // test_tree<blocked_fenwick_tree_epi32<node4>>(size_t(1) << 31);
}