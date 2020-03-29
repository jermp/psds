#pragma once

#include <vector>

namespace psds {

template <typename Node>
struct fenwick_tree_blocked {
    fenwick_tree_blocked() : m_blocks(0), m_size(0), m_ptr(nullptr) {}

    template <typename T>
    void build(T const* input, uint32_t n) {
        m_blocks = std::ceil(static_cast<double>(n) / Node::fanout);
        m_size = n;

        std::vector<int64_t> fenwick_tree_data(m_blocks);
        {
            auto sum = [&](uint64_t block) {
                int64_t s = fenwick_tree_data[block];
                uint64_t base = block * Node::fanout;
                for (uint64_t i = 1; i != Node::fanout; ++i) {
                    s += input[base + i];
                }
                return s;
            };

            for (uint64_t i = 0; i != m_blocks; ++i) {
                fenwick_tree_data[i] = input[i * Node::fanout];
            }

            for (size_t step = 2; step <= m_blocks; step *= 2) {
                for (size_t i = step - 1; i < m_blocks; i += step) {
                    fenwick_tree_data[i] += sum(i - step / 2);
                }
            }
        }

        std::vector<int64_t> node_data(Node::fanout);

        m_data.resize(m_blocks * Node::bytes);
        m_ptr = m_data.data();

        uint8_t* ptr = m_data.data();
        for (uint64_t i = 0; i != m_blocks; ++i) {
            node_data[0] = fenwick_tree_data[i];
            for (uint64_t k = 1, base = i * Node::fanout; k != Node::fanout;
                 ++k) {
                uint64_t j = base + k;
                node_data[k] = j < n ? input[j] : 0;
            }
            Node::build(node_data.data(), ptr);
            ptr += Node::bytes;
        }
    }

    static std::string name() {
        return "fenwick_tree_blocked_" + Node::name();
    }

    uint64_t blocks() const {
        return m_blocks;
    }

    uint64_t size() const {
        return m_size;
    }

    int64_t sum(uint64_t i) const {
        assert(i < size());
        uint64_t block = i / Node::fanout + 1;
        uint64_t offset = i % Node::fanout;
        int64_t s = 0;
        int64_t k = block;
        while ((k &= k - 1) != 0) {
            s += Node(m_ptr + (k - 1) * Node::bytes).sum(Node::fanout - 1);
        }
        return s + Node(m_ptr + (block - 1) * Node::bytes).sum(offset);
    }

    void update(uint64_t i, int64_t delta) {
        assert(i < size());
        uint64_t block = i / Node::fanout + 1;
        uint64_t offset = i % Node::fanout;
        uint64_t k = block;
        while ((k += k & -k) <= blocks()) {
            Node(m_ptr + (k - 1) * Node::bytes).update(0, delta);
        }
        Node(m_ptr + (block - 1) * Node::bytes).update(offset, delta);
    }

private:
    uint64_t m_blocks;
    uint32_t m_size;
    uint8_t* m_ptr;
    std::vector<uint8_t> m_data;
};

}  // namespace psds
