#pragma once

#include <vector>

template <typename T>
struct fenwick_tree {
    fenwick_tree() {}

    void build(T const* input, uint32_t n) {
        fw_tree.resize(n);
        std::copy(input, input + n, fw_tree.begin());
        for (size_t step = 1; step < fw_tree.size(); step *= 2) {
            for (size_t i = step; i + step < fw_tree.size(); i += 2 * step) {
                fw_tree[i + step] += fw_tree[i];
            }
        }
    }

    static std::string name() {
        return "fenwick_tree";
    }

    T sum(size_t i) const {
        assert(i < fw_tree.size());

        T sum = fw_tree[0];
        for (int64_t k = i; k > 0; k &= k - 1) sum += fw_tree[k];
        return sum;

        // T sum = fw_tree[0];
        // int ones = _mm_popcnt_u64(i);
        // for (int j = 0; j < ones; j++) {
        //     sum += fw_tree[i];
        //     i = i & (i - 1);
        // }
        // return sum;
    }

    void update(size_t i, T val) {
        assert(i < fw_tree.size());

        // if (i == 0)
        //     fw_tree[0] += val;
        // else {
        //     do {
        //         fw_tree[i] += val;
        //         i += i & (-i);
        //     } while (i < fw_tree.size());
        // }

        if (i == 0) {
            fw_tree[0] += val;
            return;
        }
        for (; i < fw_tree.size(); i += i & -i) fw_tree[i] += val;
    }

private:
    std::vector<T> fw_tree;
};
