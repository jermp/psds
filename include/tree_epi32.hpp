#pragma once

#include <cassert>
#include <cmath>
#include <vector>

#include "macros.hpp"

namespace psds {

template <uint32_t Height, typename Node>
struct tree_epi32 {
    tree_epi32() : m_size(0), m_num_nodes_per_level(nullptr), m_ptr(nullptr) {}

    void build(int32_t const* input, uint64_t n) {
        assert(n > 0);
        m_size = n;
        std::vector<uint32_t> num_nodes_per_level;
        uint64_t m = n;
        uint64_t total_nodes = 0;
        while (m != 1) {
            m = std::ceil(static_cast<double>(m) / Node::degree);
            assert(m < uint32_t(-1));
            num_nodes_per_level.push_back(m);
            total_nodes += m;
        }
        assert(Height == num_nodes_per_level.size());

        uint64_t total_size = total_nodes * Node::size + Height * 4;
        m_data.resize(total_size);

        m_num_nodes_per_level = reinterpret_cast<uint32_t*>(m_data.data());
        for (int h = Height - 1, i = 0; h >= 0; --h, ++i) {
            m_num_nodes_per_level[i] = num_nodes_per_level[h];
        }
        m_ptr = m_data.data() + Height * 4;

        std::vector<int32_t> tmp(Node::degree);
        uint8_t* begin = m_data.data() + total_size;  // end
        for (uint64_t h = 0, step = 1; h != Height; ++h, step *= Node::degree) {
            uint64_t nodes = num_nodes_per_level[h];
            begin -= nodes * Node::size;
            for (uint64_t i = 0, base = 0; i != nodes; ++i) {
                for (uint64_t k = 0; k != Node::degree; ++k, base += step) {
                    int32_t sum = 0;
                    for (uint64_t l = 0; l != step; ++l) {
                        sum += base + l < n ? input[base + l] : 0;
                    }
                    tmp[k] = sum;
                }
                Node::build(tmp.data(), begin + i * Node::size);
            }
        }
    }

    static std::string name() {
        return "tree_epi32_" + Node::name();
    }

    uint32_t size() const {
        return m_size;
    }

    void update(size_t i, int8_t delta) {
        assert(i < size());
        if constexpr (Height == 1) { UPDATE_H1 }
        if constexpr (Height == 2) { UPDATE_H2 }
        if constexpr (Height == 3) { UPDATE_H3 }
        if constexpr (Height == 4) { UPDATE_H4 }
        assert(false);
        __builtin_unreachable();
    }

    int64_t sum(size_t i) const {
        assert(i < size());
        if constexpr (Height == 1) { SUM_H1 }
        if constexpr (Height == 2) { SUM_H2 }
        if constexpr (Height == 3) { SUM_H3 }
        if constexpr (Height == 4) { SUM_H4 }
        assert(false);
        __builtin_unreachable();
    }

private:
    uint32_t m_size;
    uint32_t* m_num_nodes_per_level;
    uint8_t* m_ptr;
    std::vector<uint8_t> m_data;
};

}  // namespace psds