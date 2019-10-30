#include "test_common.hpp"
#include "test_tree.hpp"

template <uint32_t Degree>
struct node {
    static constexpr uint32_t degree = Degree;
    static constexpr uint32_t size = degree * 4;

    static void build(int32_t const* input, uint8_t* out) {
        std::fill(out, out + size, 0);
        int32_t* S = reinterpret_cast<int32_t*>(out);
        for (uint32_t i = 0; i != degree; ++i) S[i] = input[i];
    }

    static std::string name() {
        return "node" + std::to_string(Degree);
    }

    node(uint8_t* ptr) {
        S = reinterpret_cast<int32_t*>(ptr);
    }

    void update(uint32_t i, int8_t delta) {
        assert(i < degree);
        assert(delta == +1 or delta == -1);
        S[i] += delta;
    }

    int32_t sum(uint32_t i) const {
        assert(i < degree);
        int32_t s = 0;
        for (uint32_t k = 0; k <= i; ++k) s += S[k];
        return s;
    }

    int32_t sum_back() const {
        return sum(degree - 1);
    }

private:
    int32_t* S;
};

// template <typename Node>
// struct __blocked_fenwick_tree_epi32 {
//     __blocked_fenwick_tree_epi32() : m_size(0), m_ptr(nullptr) {}

//     void build(int32_t const* input, uint32_t n) {
//         assert(n > 0);
//         m_size = n;
//         m_blocks = std::ceil(static_cast<double>(n) / Node::degree);

//         std::vector<int32_t> fenwick_tree_data(m_blocks);
//         {
//             auto sum = [&](uint64_t block) {
//                 int32_t s = fenwick_tree_data[block];
//                 uint64_t base = block * Node::degree;
//                 for (uint64_t i = 1; i != Node::degree; ++i) {
//                     s += input[base + i];
//                 }
//                 return s;
//             };

//             for (uint64_t i = 0; i != m_blocks; ++i) {
//                 fenwick_tree_data[i] = input[i * Node::degree];
//             }

//             for (size_t step = 2; step <= m_blocks; step *= 2) {
//                 for (size_t i = step - 1; i < m_blocks; i += step) {
//                     fenwick_tree_data[i] += sum(i - step / 2);
//                 }
//             }
//         }

//         std::vector<int32_t> node_data(Node::degree);

//         m_data.resize(m_blocks * Node::size);
//         m_ptr = m_data.data();

//         uint8_t* ptr = m_data.data();
//         for (uint64_t i = 0; i != m_blocks; ++i) {
//             node_data[0] = fenwick_tree_data[i];
//             for (uint32_t k = 1, base = i * Node::degree; k != Node::degree;
//                  ++k) {
//                 uint32_t j = base + k;
//                 node_data[k] = j < n ? input[j] : 0;
//             }
//             Node::build(node_data.data(), ptr);
//             ptr += Node::size;
//         }
//     }

//     static std::string name() {
//         return "blocked_fenwick_tree_epi32_" + Node::name();
//     }

//     uint64_t blocks() const {
//         return m_blocks;
//     }

//     uint32_t size() const {
//         return m_size;
//     }

//     void update(uint32_t i, int8_t delta) {
//         assert(i < size());
//         assert(delta == +1 or delta == -1);
//         uint32_t block = i / Node::degree + 1;
//         uint32_t offset = i % Node::degree;
//         uint64_t k = block;
//         while ((k += k & -k) <= blocks()) {
//             Node(m_ptr + (k - 1) * Node::size).update(0, delta);
//         }
//         Node(m_ptr + (block - 1) * Node::size).update(offset, delta);
//     }

//     int32_t sum(uint32_t i) const {
//         assert(i < size());
//         uint32_t block = i / Node::degree + 1;
//         uint32_t offset = i % Node::degree;
//         int32_t s = 0;
//         int64_t k = block;
//         while ((k &= k - 1) != 0) {
//             s += Node(m_ptr + (k - 1) * Node::size).sum_back();
//         }
//         int32_t p = Node(m_ptr + (block - 1) * Node::size).sum(offset);
//         return s + p;
//     }

// private:
//     uint64_t m_blocks;
//     uint32_t m_size;
//     uint8_t* m_ptr;
//     std::vector<uint8_t> m_data;
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
    // test_tree<__blocked_fenwick_tree_epi32<node<16384>>>(size_t(1) << 20);
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