#pragma once

#include <cassert>
#include <cmath>
#include <vector>

#ifdef SLOW_SEGTREE
#include "slow_macros.hpp"
#else
#include "fast_macros.hpp"
#endif

namespace psds {

template <uint32_t Height, typename Node>
struct segment_tree_simd {
    segment_tree_simd()
        : m_size(0), m_num_nodes_per_level(nullptr), m_ptr(nullptr) {}

    template <typename T>
    void build(T const* input, uint64_t n) {
        assert(n > 0);
        m_size = n;
        std::vector<uint32_t> num_nodes_per_level;
        uint64_t m = n;
        uint64_t total_nodes = 0;
        while (m != 1) {
            m = std::ceil(static_cast<double>(m) / Node::fanout);
            assert(m < uint32_t(-1));
            num_nodes_per_level.push_back(m);
            total_nodes += m;
        }
        assert(Height == num_nodes_per_level.size());

#ifdef SLOW_SEGTREE
        m_height = num_nodes_per_level.size();
#endif
        uint64_t total_size = total_nodes * Node::bytes + Height * 4;
        m_data.resize(total_size);

        m_num_nodes_per_level = reinterpret_cast<uint32_t*>(m_data.data());
        for (int h = Height - 1, i = 0; h >= 0; --h, ++i) {
            m_num_nodes_per_level[i] = num_nodes_per_level[h];
        }
        m_ptr = m_data.data() + Height * 4;

        std::vector<int64_t> tmp(Node::fanout);
        uint8_t* begin = m_data.data() + total_size;  // end
        for (uint64_t h = 0, step = 1; h != Height; ++h, step *= Node::fanout) {
            uint64_t nodes = num_nodes_per_level[h];
            begin -= nodes * Node::bytes;
            for (uint64_t i = 0, base = 0; i != nodes; ++i) {
                for (uint64_t k = 0; k != Node::fanout; ++k, base += step) {
                    int64_t sum = 0;
                    for (uint64_t l = 0; l != step; ++l) {
                        sum += base + l < n ? input[base + l] : 0;
                    }
                    tmp[k] = sum;
                }

                // shift everything right by 1, except the leaves
                if (step != 1) {
                    for (int i = Node::fanout - 1; i > 0; --i) {
                        tmp[i] = tmp[i - 1];
                    }
                    tmp[0] = 0;
                }

                Node::build(tmp.data(), begin + i * Node::bytes);
            }
        }
    }

    static std::string name() {
        return "segment_tree_simd_" + Node::name();
    }

    uint32_t size() const {
        return m_size;
    }

    void update(uint64_t i, int64_t delta) {
        assert(i < size());

#ifdef SLOW_SEGTREE
        if (m_height == 1) {
            UPDATE_H1
        } else if (m_height == 2) {
            UPDATE_H2
        } else if (m_height == 3) {
            UPDATE_H3
        } else {
            UPDATE_H4
        }
#else
        if constexpr (Height == 1) { UPDATE_H1 }
        if constexpr (Height == 2) { UPDATE_H2 }
        if constexpr (Height == 3) { UPDATE_H3 }
        if constexpr (Height == 4) { UPDATE_H4 }
#endif

        assert(false);
        __builtin_unreachable();
    }

    int64_t sum(uint64_t i) const {
        assert(i < size());

#ifdef SLOW_SEGTREE
        if (m_height == 1) {
            SUM_H1
        } else if (m_height == 2) {
            SUM_H2
        } else if (m_height == 3) {
            SUM_H3
        } else {
            SUM_H4
        }
#else
        if constexpr (Height == 1) { SUM_H1 }
        if constexpr (Height == 2) { SUM_H2 }
        if constexpr (Height == 3) { SUM_H3 }
        if constexpr (Height == 4) { SUM_H4 }
#endif
        assert(false);
        __builtin_unreachable();
    }

private:
#ifdef SLOW_SEGTREE
    uint32_t m_height;
#endif
    uint32_t m_size;
    uint32_t* m_num_nodes_per_level;
    uint8_t* m_ptr;
    std::vector<uint8_t> m_data;
};

}  // namespace psds