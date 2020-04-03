#pragma once

#include "fenwick_tree.hpp"

namespace psds {

template <uint64_t ForestHeight, uint64_t TreeSize, typename ForestNode>
struct fenwick_forest {
    struct tree {  // this is a fenwick-tree of sized size

        static void build(int64_t const* in, int64_t* out) {
            std::copy(in, in + TreeSize, out);
            for (size_t step = 1; step < TreeSize; step *= 2) {
                for (size_t i = step; i + step < TreeSize; i += 2 * step) {
                    out[i + step] += out[i];
                }
            }
        }

        static int64_t sum(int64_t const* tree, uint64_t i) {
            int64_t sum = tree[0];
            for (int64_t k = i; k > 0; k &= k - 1) sum += tree[k];
            return sum;
        }

        static void update(int64_t* tree, uint64_t i, int64_t delta) {
            if (i == 0) {
                tree[0] += delta;
                return;
            }
            for (; i < TreeSize; i += i & -i) tree[i] += delta;
        }
    };

    fenwick_forest() : m_size(0) {}

    template <typename T>
    void build(T const* input, uint32_t n) {
        m_size = n;

        uint64_t num_trees = std::ceil(static_cast<double>(n) / TreeSize);
        m_fenwick_trees.resize(num_trees * TreeSize);

        std::vector<int64_t> segment_tree(num_trees);
        for (uint64_t i = 0; i != num_trees; ++i) {
            uint64_t base = i * TreeSize;
            tree::build(input + base, m_fenwick_trees.data() + base);
            int64_t sum = 0;
            for (uint64_t k = 0; k != TreeSize; ++k) sum += input[base + k];
            segment_tree[i] = sum;
        }

        m_segment_tree.build(segment_tree.data(), num_trees);
    }

    static std::string name() {
        return "fenwick_forest_" + ForestNode::name();
    }

    uint64_t size() const {
        return m_size;
    }

    int64_t sum(uint64_t i) const {
        assert(i < size());
        uint64_t tree_index = i / TreeSize;
        uint64_t within_tree_index = i % TreeSize;
        int64_t s = 0;
        if (tree_index != 0) s = m_segment_tree.sum(tree_index - 1);
        uint64_t offset = tree_index * TreeSize;
        return s +
               tree::sum(m_fenwick_trees.data() + offset, within_tree_index);
    }

    void update(uint64_t i, int64_t delta) {
        assert(i < size());
        uint64_t tree_index = i / TreeSize;
        uint64_t within_tree_index = i % TreeSize;
        m_segment_tree.update(tree_index, delta);
        uint64_t offset = tree_index * TreeSize;
        tree::update(m_fenwick_trees.data() + offset, within_tree_index, delta);
    }

private:
    uint32_t m_size;
    segment_tree_simd<ForestHeight, ForestNode> m_segment_tree;
    std::vector<int64_t> m_fenwick_trees;
};

}  // namespace psds
