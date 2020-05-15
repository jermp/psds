#pragma once

#include <vector>

namespace psds {

// struct segment_tree_bottomup {
//     template <typename T>
//     void build(T const* input, uint64_t n) {
//         uint64_t m = 2 * n - 1;
//         m_size = n;
//         m_tree.resize(m);
//         m_begin = (1ULL << uint64_t(ceil(log2(n)))) - 1;
//         uint64_t i = 0;
//         for (; m_begin + i != m; ++i) m_tree[m_begin + i] = input[i];
//         for (uint64_t j = 0; i != n; ++i, ++j) m_tree[n - 1 + j] = input[i];
//         build(0);
//     }

//     static std::string name() {
//         return "segment_tree_bottomup";
//     }

//     // static const uint64_t LEVEL3_CACHE_SIZE = 16777216;
//     // static const uint64_t T = (LEVEL3_CACHE_SIZE / sizeof(int64_t)) / 8;

//     // int64_t sum(uint64_t i) const {
//     //     uint64_t p = m_begin + i;
//     //     p -= (p >= m_tree.size()) * m_size;
//     //     int64_t sum = m_tree[p];
//     //     while (p > T) {
//     //         if ((p & 1) == 0) sum += m_tree[p - 1];
//     //         p = (p - 1) / 2;
//     //     }
//     //     while (p) {
//     //         sum += ((p & 1) == 0) * m_tree[p - 1];
//     //         p = (p - 1) / 2;
//     //     }
//     //     return sum;
//     // }

//     // int64_t sum(uint64_t i) const {
//     //     uint64_t p = m_begin + i;
//     //     p -= (p >= m_tree.size()) * m_size;
//     //     int64_t sum = m_tree[p];
//     //     if (m_size < (1ULL << 25)) {
//     //         while (p) {
//     //             sum += ((p & 1) == 0) * m_tree[p - 1];
//     //             p = (p - 1) / 2;
//     //         }
//     //     } else {
//     //         while (p) {
//     //             if ((p & 1) == 0) sum += m_tree[p - 1];
//     //             p = (p - 1) / 2;
//     //         }
//     //     }
//     //     return sum;
//     // }

//     // int64_t sum(uint64_t i) const {
//     //     uint64_t p = m_begin + i;
//     //     p -= (p >= m_tree.size()) * m_size;
//     //     int64_t sum = m_tree[p];
//     //     while (p) {
//     //         if ((p & 1) == 0) sum += m_tree[p - 1];
//     //         p = (p - 1) / 2;
//     //     }
//     //     return sum;
//     // }

//     int64_t sum(uint64_t i) const {
//         uint64_t p = m_begin + i;
//         p -= (p >= m_tree.size()) * m_size;
//         int64_t sum = m_tree[p];
//         while (p) {
//             sum += ((p & 1) == 0) * m_tree[p - 1];
//             p = (p - 1) / 2;
//         }
//         return sum;
//     }

//     void update(uint64_t i, int64_t delta) {
//         uint64_t p = m_begin + i;
//         p -= (p >= m_tree.size()) * m_size;
//         while (p) {
//             m_tree[p] += delta;
//             p = (p - 1) / 2;
//         }
//     }

// private:
//     uint64_t m_size, m_begin;
//     std::vector<int64_t> m_tree;

//     int64_t build(uint64_t p) {
//         if (p >= m_tree.size()) return 0;
//         if (p >= m_size - 1) return m_tree[p];  // leaf
//         int64_t l_sum = build(2 * p + 1);
//         int64_t r_sum = build(2 * p + 2);
//         return m_tree[p] = l_sum + r_sum;
//     }
// };

/* in this version, each internal node stores the sum of (the leaves descending
   from) its left sub-tree  */
struct segment_tree_bottomup {
    template <typename T>
    void build(T const* input, uint64_t n) {
        uint64_t m = 2 * n - 1;
        m_size = n;
        m_tree.resize(m);
        m_begin = (1ULL << uint64_t(ceil(log2(n)))) - 1;
        uint64_t i = 0;
        for (; m_begin + i != m; ++i) m_tree[m_begin + i] = input[i];
        for (uint64_t j = 0; i != n; ++i, ++j) m_tree[n - 1 + j] = input[i];
        build(0);
    }

    static std::string name() {
        return "segment_tree_bottomup";
    }

    static const uint64_t LEVEL3_CACHE_SIZE = 16777216;
    static const uint64_t T = (LEVEL3_CACHE_SIZE / sizeof(int64_t)) / 8;

    int64_t sum(uint64_t i) const {
        uint64_t p = m_begin + i;
        p -= (p >= m_tree.size()) * m_size;
        int64_t sum = m_tree[p];
        while (p > T) {
            uint64_t parent = (p - 1) / 2;
            if ((p & 1) == 0) sum += m_tree[parent];
            p = parent;
        }
        while (p) {
            uint64_t parent = (p - 1) / 2;
            sum += ((p & 1) == 0) * m_tree[parent];
            p = parent;
        }
        return sum;
    }

    // int64_t sum(uint64_t i) const {
    //     uint64_t p = m_begin + i;
    //     p -= (p >= m_tree.size()) * m_size;
    //     int64_t sum = m_tree[p];
    //     while (p) {
    //         uint64_t parent = (p - 1) / 2;
    //         if ((p & 1) == 0) sum += m_tree[parent];
    //         // sum += ((p & 1) == 0) * m_tree[parent];
    //         p = parent;
    //     }
    //     return sum;
    // }

    // void update(uint64_t i, int64_t delta) {
    //     uint64_t p = m_begin + i;
    //     p -= (p >= m_tree.size()) * m_size;
    //     m_tree[p] += delta;
    //     while (p) {
    //         uint64_t parent = (p - 1) / 2;
    //         if ((p & 1) == 1) m_tree[parent] += delta;
    //         p = parent;
    //     }
    // }

    // void update(uint64_t i, int64_t delta) {
    //     uint64_t p = m_begin + i;
    //     p -= (p >= m_tree.size()) * m_size;
    //     m_tree[p] += delta;
    //     while (p) {
    //         uint64_t parent = (p - 1) / 2;
    //         m_tree[parent] += ((p & 1) == 1) * delta;
    //         p = parent;
    //     }
    // }

    void update(uint64_t i, int64_t delta) {
        uint64_t p = m_begin + i;
        p -= (p >= m_tree.size()) * m_size;
        m_tree[p] += delta;
        while (p > T) {
            uint64_t parent = (p - 1) / 2;
            if ((p & 1) == 1) m_tree[parent] += delta;
            p = parent;
        }
        while (p) {
            uint64_t parent = (p - 1) / 2;
            m_tree[parent] += ((p & 1) == 1) * delta;
            p = parent;
        }
    }

private:
    uint64_t m_size, m_begin;
    std::vector<int64_t> m_tree;

    int64_t build(uint64_t p) {
        uint64_t l = 2 * p + 1;
        if (l >= m_tree.size()) return m_tree[p];  // leaf
        int64_t l_sum = build(l), r_sum = build(l + 1);
        m_tree[p] = l_sum;
        return l_sum + r_sum;
    }
};

}  // namespace psds