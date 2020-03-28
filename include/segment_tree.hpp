#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

namespace psds {

#define LEFT(i) 2 * i + 1
#define RIGHT(i) 2 * i + 2

struct segment_tree {
    segment_tree() : m_size(0) {}

    template <typename T>
    void build(T const* input, size_t n) {
        m_size = n;
        size_t m = size_t(1) << static_cast<size_t>(ceil(log2(n)));
        m_tree.resize(2 * m - 1);
        build(input, 0, n - 1, 0);
    }

    static std::string name() {
        return "segment_tree";
    }

    size_t size() const {
        return m_size;
    }

    int64_t sum(size_t i) const {
        size_t lo = 0;
        size_t hi = size() - 1;
        size_t pos = 0;
        int64_t sum = 0;
        while (lo < hi) {
            size_t mi = (lo + hi) / 2;
            if (i == mi) return sum + m_tree[pos];
            if (i > mi) {
                sum += m_tree[pos];
                lo = mi + 1;
                pos = RIGHT(pos);
            } else {
                hi = mi;
                pos = LEFT(pos);
            }
        }
        return sum + m_tree[pos];

        // size_t lo = 0;
        // size_t hi = size() - 1;
        // size_t pos = 0;
        // int64_t sum = 0;
        // while (lo < hi) {
        //     size_t mi = (lo + hi) / 2;
        //     size_t left = LEFT(pos);
        //     if (i == mi) return sum + m_tree[left];
        //     if (i > mi) {
        //         sum += m_tree[left];
        //         lo = mi + 1;
        //         pos = RIGHT(pos);
        //     } else {
        //         hi = mi;
        //         pos = left;
        //     }
        // }
        // return sum + m_tree[pos];
    }

    void update(size_t i, int32_t delta) {
        size_t lo = 0;
        size_t hi = size() - 1;
        size_t pos = 0;
        while (lo < hi) {
            size_t mi = (lo + hi) / 2;
            if (i > mi) {
                lo = mi + 1;
                pos = RIGHT(pos);
            } else {
                m_tree[pos] += delta;
                hi = mi;
                pos = LEFT(pos);
            }
        }
        m_tree[pos] += delta;

        // size_t lo = 0;
        // size_t hi = size() - 1;
        // size_t pos = 0;
        // while (lo < hi) {
        //     m_tree[pos] += delta;
        //     size_t mi = (lo + hi) / 2;
        //     if (i > mi) {
        //         lo = mi + 1;
        //         pos = RIGHT(pos);
        //     } else {
        //         hi = mi;
        //         pos = LEFT(pos);
        //     }
        // }
        // m_tree[pos] += delta;
    }

private:
    size_t m_size;
    std::vector<int64_t> m_tree;

    template <typename T>
    int64_t build(T const* input, size_t lo, size_t hi, size_t pos) {
        if (lo == hi) {
            m_tree[pos] = input[lo];
            return m_tree[pos];
        }
        size_t mi = (lo + hi) / 2;
        int64_t l_subtree_sum = build(input, lo, mi, LEFT(pos));
        int64_t r_subtree_sum = build(input, mi + 1, hi, RIGHT(pos));
        m_tree[pos] = l_subtree_sum;
        return l_subtree_sum + r_subtree_sum;
        // m_tree[pos] = l_subtree_sum + r_subtree_sum;
        // return m_tree[pos];
    }
};

}  // namespace psds