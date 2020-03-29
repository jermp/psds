#pragma once

#include "fenwick_tree.hpp"

namespace psds {

template <typename Node>
struct fenwick_tree_truncated {
    fenwick_tree_truncated() : m_size(0), m_ptr(nullptr) {}

    template <typename T>
    void build(T const* input, uint32_t n) {
        m_size = n;

        uint64_t nodes = std::ceil(static_cast<double>(n) / Node::fanout);
        std::vector<int64_t> fenwick_tree_data(nodes);
        std::vector<int64_t> node_data(Node::fanout);

        m_nodes.resize(nodes * Node::bytes);
        m_ptr = m_nodes.data();
        uint8_t* ptr = m_nodes.data();
        for (uint64_t i = 0; i != nodes; ++i) {
            int64_t sum = 0;
            for (uint64_t k = 0, base = i * Node::fanout; k != Node::fanout;
                 ++k) {
                uint64_t j = base + k;
                node_data[k] = j < n ? input[j] : 0;
                sum += node_data[k];
            }
            Node::build(node_data.data(), ptr);
            fenwick_tree_data[i] = sum;
            ptr += Node::bytes;
        }

        m_fenwick_tree.build(fenwick_tree_data.data(),
                             fenwick_tree_data.size());
    }

    static std::string name() {
        return "fenwick_tree_truncated_" + Node::name();
    }

    uint64_t size() const {
        return m_size;
    }

    int64_t sum(uint64_t i) const {
        assert(i < size());
        uint64_t block = i / Node::fanout;
        uint64_t offset = i % Node::fanout;
        int64_t s = 0;
        if (block > 0) s += m_fenwick_tree.sum(block - 1);
        return s + Node(m_ptr + block * Node::bytes).sum(offset);
    }

    void update(uint64_t i, int64_t delta) {
        assert(i < size());
        uint64_t block = i / Node::fanout;
        uint64_t offset = i % Node::fanout;
        m_fenwick_tree.update(block, delta);
        Node(m_ptr + block * Node::bytes).update(offset, delta);
    }

private:
    uint32_t m_size;
    fenwick_tree m_fenwick_tree;
    uint8_t* m_ptr;
    std::vector<uint8_t> m_nodes;
};

}  // namespace psds
