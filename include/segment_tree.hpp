#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

namespace psds {

#define LEFT(i) 2 * i + 1
#define RIGHT(i) 2 * i + 2

template <typename T>
struct segment_tree {
    void build(T const* input, size_t n) {
        m_size = n;
        size_t m = size_t(1) << static_cast<size_t>(ceil(log2(n)));
        m_tree.resize(2 * m - 1);
        build(input, 0, n - 1, 0);
    }

    T build(T const* input, size_t lo, size_t hi, size_t pos) {
        if (lo == hi) {
            m_tree[pos] = input[lo];
            return m_tree[pos];
        }
        size_t mi = (lo + hi) / 2;
        auto l_subtree_sum = build(input, lo, mi, LEFT(pos));
        auto r_subtree_sum = build(input, mi + 1, hi, RIGHT(pos));
        m_tree[pos] = l_subtree_sum;
        return l_subtree_sum + r_subtree_sum;
    }

    segment_tree() {}

    static std::string name() {
        return "segment_tree";
    }

    // void print() const {
    //     for (auto x : m_tree) { std::cout << x << " "; }
    //     std::cout << std::endl;
    // }

    size_t size() const {
        return m_size;
    }

    T sum(size_t i) const {
        // return sum(i, 0, size() - 1, 0); // recursive

        size_t lo = 0;
        size_t hi = size() - 1;
        size_t pos = 0;
        T s = 0;
        while (lo < hi) {
            size_t mi = (lo + hi) / 2;
            if (i == mi) return s + m_tree[pos];
            if (i > mi) {
                s += m_tree[pos];
                lo = mi + 1;
                pos = RIGHT(pos);
            } else {
                hi = mi;
                pos = LEFT(pos);
            }
        }
        return s + m_tree[pos];
    }

    void update(size_t i, T delta) {
        // update(i, delta, 0, size() - 1, 0);  // recursive

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
    }

private:
    size_t m_size;
    std::vector<T> m_tree;

    // T sum(size_t i, size_t lo, size_t hi, size_t pos) const {
    //     if (lo == hi) return m_tree[pos];
    //     size_t mi = (lo + hi) / 2;
    //     if (i == mi) return m_tree[pos];
    //     if (i > mi) return m_tree[pos] + sum(i, mi + 1, hi, RIGHT(pos));
    //     return sum(i, lo, mi, LEFT(pos));
    // }

    // void update(size_t i, T delta, size_t lo, size_t hi, size_t pos) {
    //     if (lo == hi) {
    //         m_tree[pos] += delta;
    //         return;
    //     }
    //     size_t mi = (lo + hi) / 2;
    //     if (i <= mi) {
    //         m_tree[pos] += delta;
    //         update(i, delta, lo, mi, LEFT(pos));
    //         return;
    //     }
    //     update(i, delta, mi + 1, hi, RIGHT(pos));
    // }
};
}  // namespace psds