#pragma once

#include <vector>

namespace psds {

template <typename T>
struct fenwick_tree {
    fenwick_tree() {}

    void build(T const* input, uint32_t n) {
        m_data.resize(n);
        std::copy(input, input + n, m_data.begin());
        for (size_t step = 1; step < m_data.size(); step *= 2) {
            for (size_t i = step; i + step < m_data.size(); i += 2 * step) {
                m_data[i + step] += m_data[i];
            }
        }
    }

    static std::string name() {
        return "fenwick_tree";
    }

    size_t size() const {
        return m_data.size();
    }

    T sum(size_t i) const {
        assert(i < size());
        T sum = m_data[0];
        for (int64_t k = i; k > 0; k &= k - 1) sum += m_data[k];
        return sum;
    }

    void update(size_t i, T delta) {
        assert(i < size());
        if (i == 0) {
            m_data[0] += delta;
            return;
        }
        for (; i < size(); i += i & -i) m_data[i] += delta;
    }

private:
    std::vector<T> m_data;
};

}  // namespace psds
