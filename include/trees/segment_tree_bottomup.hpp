#pragma once

#include <vector>

namespace psds {

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

    int64_t sum(uint64_t i) const {
        uint64_t p = m_begin + i;
        p -= (p >= m_tree.size()) * m_size;
        int64_t sum = m_tree[p];
        while (p) {
            uint64_t pp = (p - 1) / 2;  // parent
            if (((p + 1) & 3) == 0) {
                pp = (pp - 1) / 2;  // grand parent
            } else if ((p & 1) == 0) {
                sum += m_tree[pp];  // p is right child
            }
            // sum += ((p & 1) == 0) * m_tree[pp];
            p = pp;
        }
        return sum;
    }

    void update(uint64_t i, int64_t delta) {
        uint64_t p = m_begin + i;
        p -= (p >= m_tree.size()) * m_size;
        m_tree[p] += delta;
        while (p) {
            uint64_t pp = (p - 1) / 2;
            if ((p & 1) == 1) m_tree[pp] += delta;  // p is left child
            // m_tree[pp] += ((p & 1) == 1) * delta;
            p = pp;
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