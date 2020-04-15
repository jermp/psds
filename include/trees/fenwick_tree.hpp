#pragma once

#include <vector>

namespace psds {

struct fenwick_tree {
    fenwick_tree() {}

    template <typename T>
    void build(T const* input, uint64_t n) {
        m_tree.resize(n + 1, 0);
        std::copy(input, input + n, m_tree.begin() + 1);
        for (size_t step = 1; step < m_tree.size(); step *= 2) {
            for (size_t i = step; i + step < m_tree.size(); i += 2 * step) {
                m_tree[i + step] += m_tree[i];
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
        for (++i; i != 0; i &= i - 1) sum += m_tree[i];
        return sum;
    }

    void update(uint64_t i, int64_t delta) {
        assert(i < size());
        for (++i; i < size(); i += i & -i) m_tree[i] += delta;
    }

private:
    std::vector<int64_t> m_tree;
};

}  // namespace psds
