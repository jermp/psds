#include "test_common.hpp"
#include "test_tree.hpp"

// struct segment_tree1 {
//     template <typename T>
//     void build(T const* input, size_t n) {
//         m_size = size_t(1) << static_cast<size_t>(ceil(log2(n)));
//         m_tree.resize(2 * m_size - 1, 0);
//         std::vector<int64_t> in(m_size, 0);
//         for (uint64_t i = 0; i != n; ++i) in[i] = input[i];
//         build(in.data(), 0, m_size - 1, 0);
//     }

//     static std::string name() {
//         return "segment_tree1";
//     }

//     int64_t sum(uint64_t i) const {
//         if ((m_size + 1) / 2 < 16777216) {  // go branch-less here...
//             uint64_t n = m_size;
//             uint64_t m = (n - 1) / 2;
//             uint64_t pos = 0;
//             int64_t sum = 0;
//             while (n != 1) {
//                 uint64_t cmp = i > m;
//                 sum += cmp * m_tree[pos];
//                 pos = (pos << 1) + cmp + 1;
//                 n >>= 1;
//                 int64_t offset = cmp * n - n / 2;
//                 m += offset;
//             }
//             return sum + m_tree[pos];
//         }
//         // but switch to branchy code for large n
//         size_t lo = 0;
//         size_t hi = m_size - 1;
//         size_t p = 0;
//         int64_t sum = 0;
//         while (lo < hi) {
//             size_t mi = (lo + hi) / 2;
//             if (i == mi) break;
//             if (i > mi) {
//                 sum += m_tree[p];
//                 lo = mi + 1;
//                 p = 2 * p + 2;
//             } else {
//                 hi = mi;
//                 p = 2 * p + 1;
//             }
//         }
//         return sum + m_tree[p];
//     }

//     void update(uint64_t i, int64_t delta) {
//         if ((m_size + 1) / 2 < 16777216) {
//             uint64_t n = m_size;
//             uint64_t mi = (m_size - 1) / 2;
//             uint64_t p = 0;
//             while (n != 1) {
//                 uint64_t cmp = i > mi;
//                 m_tree[p] += !cmp * delta;
//                 p = (p << 1) + cmp + 1;
//                 n >>= 1;
//                 int64_t offset = cmp * n - n / 2;
//                 mi += offset;
//             }
//             m_tree[p] += delta;
//             return;
//         }
//         size_t lo = 0;
//         size_t hi = m_size - 1;
//         size_t p = 0;
//         while (lo < hi) {
//             size_t mi = (lo + hi) / 2;
//             if (i > mi) {
//                 lo = mi + 1;
//                 p = 2 * p + 2;
//             } else {
//                 m_tree[p] += delta;
//                 hi = mi;
//                 p = 2 * p + 1;
//             }
//         }
//         m_tree[p] += delta;
//     }

// private:
//     size_t m_size;
//     std::vector<int64_t> m_tree;

//     template <typename T>
//     int64_t build(T const* input, size_t lo, size_t hi, size_t p) {
//         if (lo == hi) {
//             m_tree[p] = input[lo];
//             return m_tree[p];
//         }
//         size_t mi = (lo + hi) / 2;
//         int64_t l_subtree_sum = build(input, lo, mi, 2 * p + 1);
//         int64_t r_subtree_sum = build(input, mi + 1, hi, 2 * p + 2);
//         m_tree[p] = l_subtree_sum;
//         return l_subtree_sum + r_subtree_sum;
//     }
// };

TEST_CASE("test segment_tree") {
    uint64_t n = sizeof(sizes) / sizeof(sizes[0]);
    for (uint64_t i = 0; i != n; ++i) test_tree<segment_tree>(sizes[i]);
}
