#pragma once

#include <vector>

namespace psds {

struct fenwick_tree {
    fenwick_tree() {}

    // template <typename T>
    // void build(T const* input, uint64_t n) {
    //     m_tree.resize(n + 1, 0);
    //     std::copy(input, input + n, m_tree.begin() + 1);
    //     for (size_t step = 2; step <= n; step *= 2)
    //         for (size_t i = step; i <= n; i += step)
    //             m_tree[i] += m_tree[i - step / 2];
    // }

    template <typename T>
    void build(T const* input, uint64_t n) {
        m_size = n;
        m_tree.resize(pos(n) + 1, 0);
        for (size_t i = 1; i <= n; i++) m_tree[pos(i)] = input[i - 1];
        for (size_t step = 2; step <= n; step *= 2) {
            for (size_t i = step; i <= n; i += step) {
                m_tree[pos(i)] += m_tree[pos(i - step / 2)];
            }
        }
    }

    static std::string name() {
        return "fenwick_tree";
    }

    uint64_t size() const {
        return m_tree.size();
    }

    int64_t sum(uint64_t i) const {
        assert(i < size());
        int64_t sum = 0;
        // for (++i; i != 0; i &= i - 1) sum += m_tree[i];
        for (++i; i != 0; i &= i - 1) sum += m_tree[pos(i)];
        return sum;
    }

    void update(uint64_t i, int64_t delta) {
        assert(i < size());
        // for (++i; i < size(); i += i & -i) m_tree[i] += delta;
        for (++i; i <= m_size; i += i & -i) m_tree[pos(i)] += delta;
    }

private:
    uint64_t m_size;
    std::vector<int64_t> m_tree;

    static inline uint64_t pos(uint64_t i) {
        return i + (i >> 14);
    }
};

}  // namespace psds
