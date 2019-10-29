#pragma once

#include <vector>

#include "fenwick_tree.hpp"

namespace psds {

template <typename Node>
struct truncated_fenwick_tree_epi32 {
    truncated_fenwick_tree_epi32() : m_size(0), m_ptr(nullptr) {}

    void build(int32_t const* input, uint32_t n) {
        assert(n > 0);
        m_size = n;

        uint32_t nodes = std::ceil(static_cast<double>(n) / Node::degree);
        std::vector<int32_t> fenwick_tree_data(nodes);
        std::vector<int32_t> node_data(Node::degree);

        m_data.resize(nodes * Node::size);
        m_ptr = m_data.data();

        uint8_t* ptr = m_data.data();
        for (uint32_t i = 0; i != nodes; ++i) {
            int32_t sum = 0;
            for (uint32_t k = 0, base = i * Node::degree; k != Node::degree;
                 ++k) {
                uint32_t j = base + k;
                node_data[k] = j < n ? input[j] : 0;
                sum += node_data[k];
            }
            Node::build(node_data.data(), ptr);
            fenwick_tree_data[i] = sum;
            ptr += Node::size;
        }

        m_fenwick_tree.build(fenwick_tree_data.data(),
                             fenwick_tree_data.size());
    }

    static std::string name() {
        return "truncated_fenwick_tree_epi32_" + Node::name();
    }

    uint32_t size() const {
        return m_size;
    }

    void update(uint32_t i, int8_t delta) {
        assert(i < size());
        assert(delta == +1 or delta == -1);
        uint32_t block = i / Node::degree;
        uint32_t offset = i % Node::degree;
        m_fenwick_tree.update(block, delta);
        return Node(m_ptr + block * Node::size).update(offset, delta);
    }

    int32_t sum(uint32_t i) const {
        assert(i < size());
        uint32_t block = i / Node::degree;
        uint32_t offset = i % Node::degree;
        int32_t s = 0;
        if (block > 0) s += m_fenwick_tree.sum(block - 1);
        return s + Node(m_ptr + block * Node::size).sum(offset);
    }

private:
    uint32_t m_size;
    fenwick_tree<int32_t> m_fenwick_tree;
    uint8_t* m_ptr;
    std::vector<uint8_t> m_data;
};

}  // namespace psds
