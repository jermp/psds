#pragma once

#include <cassert>
#include <cmath>
#include <vector>

#include "macros.hpp"

namespace dps {

template <typename Node>
struct tree_epi32 {
    tree_epi32()
        : m_size(0)
        , m_height(0)
        , m_num_nodes_per_level(nullptr)
        , m_ptr(nullptr) {}

    void build(int32_t const* input, uint32_t n) {
        assert(n > 0);
        m_size = n;
        std::vector<uint32_t> num_nodes_per_level;
        uint32_t m = n;
        uint32_t total_nodes = 0;
        while (m != 1) {
            m = std::ceil((double)m / Node::degree);
            num_nodes_per_level.push_back(m);
            total_nodes += m;
        }

        m_height = num_nodes_per_level.size();
        // std::cout << "m_height " << m_height << std::endl;
        size_t total_size = total_nodes * Node::size + m_height * 4;
        // std::cout << "total_size " << total_size << std::endl;
        m_data.resize(total_size);

        m_num_nodes_per_level = reinterpret_cast<uint32_t*>(m_data.data());
        for (int h = m_height - 1, i = 0; h >= 0; --h, ++i) {
            m_num_nodes_per_level[i] = num_nodes_per_level[h];
        }
        m_ptr = m_data.data() + m_height * 4;

        typename Node::builder builder;
        std::vector<int32_t> tmp(Node::degree);
        uint8_t* begin = m_data.data() + total_size;

        for (uint32_t h = 0, step = 1; h != m_height;
             ++h, step *= Node::degree) {
            uint32_t nodes = num_nodes_per_level[h];
            // std::cout << "nodes = " << nodes << std::endl;
            // std::cout << "step = " << step << std::endl;
            begin -= nodes * Node::size;
            for (uint32_t i = 0, base = 0; i != nodes; ++i) {
                for (uint32_t k = 0; k != Node::degree; ++k, base += step) {
                    int32_t sum = 0;
                    for (uint32_t l = 0; l != step; ++l) {
                        sum += base + l < n ? input[base + l] : 0;
                    }
                    tmp[k] = sum;
                }
                builder.build(tmp.data(), begin + i * Node::size);
            }
        }
    }

    static std::string name() {
        return "tree_epi32_" + Node::name();
    }

    uint32_t size() const {
        return m_size;
    }

    void update(uint32_t i, int8_t delta) {
        assert(i < size());

        switch (m_height) {
            case 1: {
                UPDATE_H1 break;
            }
            case 2: {
                UPDATE_H2(Node::degree) break;
            }
            case 3: {
                UPDATE_H3(Node::degree) break;
            }
            case 4: {
                UPDATE_H4(Node::degree) break;
            }
            default:
                assert(false);
                __builtin_unreachable();
        }
    }

    int32_t sum(uint32_t i) const {
        assert(i < size());

        switch (m_height) {  // TODO: this should be known at compile time, so
                             // that we can use if constexpr
            case 1: {
                SUM_H1 return s1;
            }
            case 2: {
                SUM_H2(Node::degree) return s1 + s2;
            }
            case 3: {
                SUM_H3(Node::degree) return s1 + s2 + s3;
            }
            case 4: {
                SUM_H4(Node::degree) return s1 + s2 + s3 + s4;
            }
            default:
                assert(false);
                __builtin_unreachable();
        }

        return 0;
    }

private:
    uint32_t m_size;
    uint32_t m_height;
    uint32_t* m_num_nodes_per_level;
    uint8_t* m_ptr;
    std::vector<uint8_t> m_data;
};

}  // namespace dps