#pragma once

#include <vector>

namespace psds {

template <typename Node>
struct blocked_fenwick_tree_epi32 {
    blocked_fenwick_tree_epi32() : m_size(0), m_ptr(nullptr) {}

    void build(int32_t const* input, uint32_t n) {
        assert(n > 0);
        m_size = n;
        m_blocks = std::ceil(static_cast<double>(n) / Node::degree);

        std::vector<int32_t> fenwick_tree_data(m_blocks);
        {
            auto sum = [&](uint64_t block) {
                int32_t s = fenwick_tree_data[block];
                uint64_t base = block * Node::degree;
                for (uint64_t i = 1; i != Node::degree; ++i) {
                    s += input[base + i];
                }
                return s;
            };

            for (uint64_t i = 0; i != m_blocks; ++i) {
                fenwick_tree_data[i] = input[i * Node::degree];
            }

            for (size_t step = 2; step <= m_blocks; step *= 2) {
                for (size_t i = step - 1; i < m_blocks; i += step) {
                    fenwick_tree_data[i] += sum(i - step / 2);
                }
            }
        }

        std::vector<int32_t> node_data(Node::degree);

        m_data.resize(m_blocks * Node::size);
        m_ptr = m_data.data();

        uint8_t* ptr = m_data.data();
        for (uint64_t i = 0; i != m_blocks; ++i) {
            node_data[0] = fenwick_tree_data[i];
            for (uint32_t k = 1, base = i * Node::degree; k != Node::degree;
                 ++k) {
                uint32_t j = base + k;
                node_data[k] = j < n ? input[j] : 0;
            }
            Node::build(node_data.data(), ptr);
            ptr += Node::size;
        }
    }

    static std::string name() {
        return "blocked_fenwick_tree_epi32_" + Node::name();
    }

    uint64_t blocks() const {
        return m_blocks;
    }

    uint32_t size() const {
        return m_size;
    }

    void update(uint32_t i, int8_t delta) {
        assert(i < size());
        assert(delta == +1 or delta == -1);
        uint32_t block = i / Node::degree + 1;
        uint32_t offset = i % Node::degree;
        uint64_t k = block;
        while ((k += k & -k) <= blocks()) {
            Node(m_ptr + (k - 1) * Node::size).update(0, delta);
        }
        Node(m_ptr + (block - 1) * Node::size).update(offset, delta);
    }

    int32_t sum(uint32_t i) const {
        assert(i < size());
        uint32_t block = i / Node::degree + 1;
        uint32_t offset = i % Node::degree;
        int32_t s = 0;
        int64_t k = block;
        while ((k &= k - 1) > 0) {
            s += Node(m_ptr + (k - 1) * Node::size).sum(Node::degree - 1);
        }
        s += Node(m_ptr + (block - 1) * Node::size).sum(offset);
        return s;
    }

private:
    uint64_t m_blocks;
    uint32_t m_size;
    uint8_t* m_ptr;
    std::vector<uint8_t> m_data;
};

}  // namespace psds
