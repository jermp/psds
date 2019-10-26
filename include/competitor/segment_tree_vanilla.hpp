#pragma once

#include <cmath>
#include <vector>
#include <cassert>

#define left(index) 2 * index + 1
#define right(index) 2 * index + 2

template <typename T>
struct segment_tree_vanilla {
    segment_tree_vanilla()
        : height(0)
        , max_breadth(0)
        , second_last_level_size(0)
        , last_level_size(0)
        , gamma(0) {}

    void build(T const* input, uint32_t n) {
        data.resize(n);
        std::copy(input, input + n, data.begin());

        tree.resize(n - 1);

        // Height of the tree (root is at height 0)
        height = std::ceil(std::log2(n));
        // Max number of leaves for the height (or the max number of nodes of
        // the tree +1)
        max_breadth = std::pow(2, height);
        // Numbers of nodes in the last and second-last levels of the tree
        last_level_size = tree.size() - (std::pow(2, height - 1) - 1);
        second_last_level_size = std::pow(2, height - 2);
        // Position, in the second last level, of the last node having at least
        // another node as child
        gamma = (last_level_size - 1) / 2;
        assert(gamma < std::pow(2, height - 2));

        /* == Fill tree vector == */
        size_t index = tree.size() - 1;
        // Fill the last level of the tree. Every node is connected with leaves
        for (size_t j = 0; j < last_level_size; j++) {
            tree[index] = data[left(index) - (max_breadth - 1)];
            tree[index] += data[right(index) - (max_breadth - 1)];
            index--;
        }

        // If the tree is not balanced, second last level nodes might have
        // leaves as successors
        if (max_breadth != n) {
            int n = std::pow(2, height - 1) - 1;
            int m = std::pow(2, height - 2);
            for (int i = 0; i < m; i++) {
                size_t l = left(index);
                size_t r = right(index);

                tree[index] =
                    (l < tree.size()) ? tree[l] : data[l - n + last_level_size];
                tree[index] +=
                    (r < tree.size()) ? tree[r] : data[r - n + last_level_size];

                index--;
            }
        }

        // From now on all the nodes have other nodes as successors
        for (; index < tree.size(); index--) {
            tree[index] = tree[left(index)] + tree[right(index)];
        }
        /* == tree vector filled == */
    }

    static std::string name() {
        return "segment_tree_vanilla";
    }

    T sum(size_t i) const {
        T sum = 0;

        size_t a = ancestor(i);
        assert(a >= 0 && a < pow(2, height - 2));

        size_t start = 0, end = second_last_level_size - 1;
        size_t index = 0;
        // Compute the sum as if the tree ended at the second-last level
        // It can be considered as always balanced
        for (size_t j = 0; j < height - 1; j++) {
            assert(start <= a && a <= end);
            if (a == end) {
                sum += tree[index];
                break;
            }

            size_t mid = (start + end) / 2;
            if (a > mid) {
                sum += tree[left(index)];
                index = right(index);
                start = mid + 1;
            } else {
                index = left(index);
                end = mid;
            }
        }

        size_t ei = ancestor_interval_end(a);
        assert(ei >= i && ei <= i + 3);

        // The previous for could have added to sum some values, connected to
        // 'a', that are outside the [0,i] interval
        for (; ei > i; ei--) {
            sum -= data[ei];
        }

        return sum;
    }

    // Update the value at index i adding val
    void update(size_t i, T val) {
        assert(i < data.size());
        data[i] += val;
        size_t a = ancestor(i);
        assert(a < pow(2, height - 2));

        size_t start = 0, end = second_last_level_size - 1;
        size_t index = 0;
        // Since last level nodes are never accessed, there's no need
        // to update them
        for (size_t j = 0; j < height - 1; j++) {
            size_t mid = (start + end) / 2;

            tree[index] += val;

            if (a > mid) {
                start = mid + 1;
                index = right(index);
            } else {
                end = mid;
                index = left(index);
            }
        }
    }

private:
    // Calculate the positon, in the second-last level of the tree, of the node
    // which has the element i in his subtree
    inline size_t ancestor(size_t index) const {
        return (index < 2 * last_level_size) ? index / 4
                                             : (index - last_level_size) / 2;
    }

    // Calculate the end of the interval underlying the second-last level
    // node in position a
    inline size_t ancestor_interval_end(size_t a) const {
        return 2 * a + 1 + ((a < gamma) ? 2 * a + 2 : last_level_size);
    }

    std::vector<T> tree;
    std::vector<T> data;
    size_t height, max_breadth;
    size_t second_last_level_size, last_level_size;
    size_t gamma;
};