#pragma once

#include <vector>

#include "util.hpp"

namespace psds {

#define DIGIT(K)        \
    int64_t sum##K = 0; \
    uint64_t digit##K = (i >> ((K - 1) * log2_b)) & group_mask;

#define DIGIT_1 DIGIT(1)
#define DIGIT_2 \
    DIGIT_1     \
    DIGIT(2)
#define DIGIT_3 \
    DIGIT_2     \
    DIGIT(3)
#define DIGIT_4 \
    DIGIT_3     \
    DIGIT(4)
#define DIGIT_5 \
    DIGIT_4     \
    DIGIT(5)

#define SUM_DIGIT(K)                                                \
    if (digit##K != 0) {                                            \
        sum##K = Node(m_ptr + (block##K - 1) * Node::bytes).back(); \
    }

#define SUM_DIGIT_1                                                      \
    if (digit1 != 0) {                                                   \
        sum1 = Node(m_ptr + (block1 - 1) * Node::bytes).sum(digit1 - 1); \
    }

#define SUM_DIGIT_2 \
    SUM_DIGIT(2)    \
    SUM_DIGIT_1

#define SUM_DIGIT_3 \
    SUM_DIGIT(3)    \
    SUM_DIGIT_2

#define SUM_DIGIT_4 \
    SUM_DIGIT(4)    \
    SUM_DIGIT_3

#define SUM_DIGIT_5 \
    SUM_DIGIT(5)    \
    SUM_DIGIT_4

template <uint32_t Height, typename Node>
struct fenwick_tree_bary {
    static_assert(Height > 0);
    typedef fenwick_tree_bary<Height, Node> tree_type;

    fenwick_tree_bary() : m_size(0), m_ptr(nullptr) {}

    static const uint64_t b = Node::fanout;
    static_assert((b & (b - 1)) == 0, "Node's fanout must be a power of 2");
    static const uint64_t log2_b = util::floor_log2(b);
    static const uint64_t group_mask = (1ULL << log2_b) - 1;

    template <typename T>
    void build(T const* input, uint64_t n) {
        uint64_t blocks = std::ceil(static_cast<double>(n) / b) + 1;
        m_size = n;
        std::vector<int64_t> fenwick_tree_data(1 + blocks * b, 0);
        std::copy(input, input + n, fenwick_tree_data.begin() + 1);
        uint64_t size = fenwick_tree_data.size();
        build(fenwick_tree_data.data(), 1, size - 1, size - 1);

        std::vector<int64_t> node_data(b);
        m_data.resize(blocks * Node::bytes, 0);
        m_ptr = m_data.data();
        uint8_t* ptr = m_data.data();
        for (uint64_t i = 0, begin = 1; i != blocks; ++i, begin += b) {
            for (uint64_t k = 0; k != b; ++k) {
                node_data[k] = fenwick_tree_data[begin + k];
            }
            Node::build(node_data.data(), ptr);
            ptr += Node::bytes;
        }
    }

    static std::string name() {
        return "fenwick_tree_bary_" + Node::name();
    }

    uint64_t size() const {
        return m_size;
    }

    int64_t sum(uint64_t i) const {
        assert(i < size());
        i += 1;

        if constexpr (Height == 1) {
            uint64_t digit1 = i & group_mask;
            return Node(m_ptr).sum(digit1 - 1);
        }

        if constexpr (Height == 2) {
            DIGIT_2
            uint64_t block2 = digit2;
            uint64_t block1 = block2 + 1;
            SUM_DIGIT_2
            return sum1 + sum2;
        }

        if constexpr (Height == 3) {
            DIGIT_3
            uint64_t block3 = digit3 * b;
            uint64_t block2 = block3 + digit2;
            uint64_t block1 = block2 + 1;
            SUM_DIGIT_3
            return sum1 + sum2 + sum3;
        }

        if constexpr (Height == 4) {
            DIGIT_4
            uint64_t block4 = digit4 * b * b;
            uint64_t block3 = block4 + digit3 * b;
            uint64_t block2 = block3 + digit2;
            uint64_t block1 = block2 + 1;
            SUM_DIGIT_4
            return sum1 + sum2 + sum3 + sum4;
        }

        if constexpr (Height == 5) {
            DIGIT_5
            uint64_t block5 = digit5 * b * b * b;
            uint64_t block4 = block5 + digit4 * b * b;
            uint64_t block3 = block4 + digit3 * b;
            uint64_t block2 = block3 + digit2;
            uint64_t block1 = block2 + 1;
            SUM_DIGIT_5
            return sum1 + sum2 + sum3 + sum4 + sum5;
        }
    }

    void update(uint64_t i, int64_t delta) {
        assert(i < size());

        uint64_t block = i / b;
        uint64_t digit = i & group_mask;
        Node node(m_ptr + block * Node::bytes);
        node.update(digit, delta);
        if (digit != b - 1) node.update_back(delta);

        for (uint64_t power = b, group = 1, offset = block * b; group < Height;
             power *= b, ++group) {
            i >>= log2_b;
            uint64_t digit = i & group_mask;
            uint64_t powers = b - digit - 1;
            for (uint64_t p = 0; p != powers; ++p) {
                offset += power;
                if (offset >= m_size) break;
                uint64_t block = offset / b;
                Node(m_ptr + block * Node::bytes).update_back(delta);
            }
        }
    }

private:
    uint64_t m_size;
    uint8_t* m_ptr;
    std::vector<uint8_t> m_data;

    void build(int64_t* input, uint64_t l, uint64_t r, uint64_t size) {
        assert(r > l);
        int64_t sum = 0;
        for (uint64_t power = b, begin = l; power < r - l + 1; power *= b) {
            uint64_t powers = (b * (power + 1)) / power - 1;
            uint64_t end = 0;
            for (uint64_t i = 0; i != powers and end != size; ++i) {
                end = begin + power - 1;
                if (end <= r) {
                    for (uint64_t j = begin; j <= end; ++j) sum += input[j];
                    input[end] = sum;
                }
                if (end > size) end = size;
                build(input, begin, end - 1, size);
                begin = end + 1;
            }
        }
    }
};

}  // namespace psds