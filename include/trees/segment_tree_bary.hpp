#pragma once

#include <cassert>
#include <cmath>
#include <vector>

#include "macros.hpp"

namespace psds {

template <uint32_t Height, typename Node>
struct segment_tree_bary {
    static_assert(Height > 0);
    typedef segment_tree_bary<Height, Node> tree_type;

    segment_tree_bary()
        : m_size(0), m_num_nodes_per_level(nullptr), m_ptr(nullptr) {}

    template <typename T>
    void build(T const* input, uint64_t n) {
        assert(n > 0);
        m_size = n;
        std::vector<uint32_t> num_nodes_per_level(Height);
        uint64_t m = n;
        uint64_t total_nodes = 0;
        for (int h = Height - 1; h >= 0; --h) {
            m = std::ceil(static_cast<double>(m) / Node::fanout);
            num_nodes_per_level[h] = m;
            total_nodes += m;
        }
        assert(m == 1);

        uint64_t total_size =
            total_nodes * Node::bytes + Height * sizeof(uint32_t);
        m_data.resize(total_size);

        m_num_nodes_per_level = reinterpret_cast<uint32_t*>(m_data.data());
        for (uint32_t i = 0; i != Height; ++i) {
            m_num_nodes_per_level[i] = num_nodes_per_level[i];
        }
        m_ptr = m_data.data() + Height * sizeof(uint32_t);

        std::vector<int64_t> tmp(Node::fanout);
        uint8_t* begin = m_data.data() + total_size;  // end
        uint64_t step = 1;
        for (int h = Height - 1; h >= 0; --h, step *= Node::fanout) {
            uint64_t nodes = num_nodes_per_level[h];
            begin -= nodes * Node::bytes;
            uint8_t* out = begin;
            for (uint64_t i = 0, base = 0; i != nodes; ++i) {
                for (int64_t& x : tmp) {
                    int64_t sum = 0;
                    for (uint64_t k = 0; k != step and base + k < n; ++k) {
                        sum += input[base + k];
                    }
                    x = sum;
                    base += step;
                }

                // shift everything right by 1, except the leaves
                if (step != 1) {
                    for (int i = Node::fanout - 1; i > 0; --i) {
                        tmp[i] = tmp[i - 1];
                    }
                    tmp[0] = 0;
                }

                Node::build(tmp.data(), out);
                out += Node::bytes;
            }
        }
    }

    static std::string name() {
        return "segment_tree_bary_" + Node::name();
    }

    uint32_t size() const {
        return m_size;
    }

    int64_t sum(uint64_t i) const {
        assert(i < size());
        if constexpr (Height == 1) { SUM_H1 }
        if constexpr (Height == 2) { SUM_H2 }
        if constexpr (Height == 3) { SUM_H3 }
        if constexpr (Height == 4) { SUM_H4 }
        if constexpr (Height == 5) { SUM_H5 }
    }

    void update(uint64_t i, int64_t delta) {
        assert(i < size());
        if constexpr (Height == 1) { UPDATE_H1 }
        if constexpr (Height == 2) { UPDATE_H2 }
        if constexpr (Height == 3) { UPDATE_H3 }
        if constexpr (Height == 4) { UPDATE_H4 }
        if constexpr (Height == 5) { UPDATE_H5 }
    }

private:
    uint32_t m_size;
    uint32_t* m_num_nodes_per_level;
    uint8_t* m_ptr;
    std::vector<uint8_t> m_data;
};

}  // namespace psds