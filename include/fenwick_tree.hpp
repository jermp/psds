#pragma once

#include <vector>

namespace psds {

struct fenwick_tree {
    fenwick_tree() {}

    template <typename T>
    void build(T const* input, uint32_t n) {
        m_tree.resize(n);
        std::copy(input, input + n, m_tree.begin());
        for (size_t step = 1; step < m_tree.size(); step *= 2) {
            for (size_t i = step; i + step < m_tree.size(); i += 2 * step) {
                m_tree[i + step] += m_tree[i];
            }
        }
    }

    static std::string name() {
        return "fenwick_tree";
    }

    size_t size() const {
        return m_tree.size();
    }

    int64_t sum(size_t i) const {
        assert(i < size());
        int64_t sum = m_tree[0];
        for (int64_t k = i; k > 0; k &= k - 1) sum += m_tree[k];
        return sum;
    }

    void update(size_t i, int32_t delta) {
        assert(i < size());
        if (i == 0) {
            m_tree[0] += delta;
            return;
        }
        for (; i < size(); i += i & -i) m_tree[i] += delta;
    }

private:
    std::vector<int64_t> m_tree;
};

}  // namespace psds
