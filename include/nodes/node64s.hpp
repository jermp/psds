#pragma once

#include <cassert>

#include "immintrin.h"
#include "common.hpp"
#include "tables.hpp"

namespace psds {

struct node64s {
    static constexpr uint64_t fanout = 64;
    static constexpr uint64_t segments = 8;
    static constexpr uint64_t summary_bytes = segments * sizeof(int64_t);
    static constexpr uint64_t bytes = summary_bytes + fanout * sizeof(int64_t);

    node64s() {}  // do not initialize

    template <typename T>
    static void build(T const* input, uint8_t* out) {
        build_node(input, out, fanout, segments, summary_bytes, bytes);
    }

    static std::string name() {
        return "node64s";
    }

    node64s(uint8_t* ptr) {
        at(ptr);
    }

    inline void at(uint8_t* ptr) {
        summary = reinterpret_cast<int64_t*>(ptr);
        keys = reinterpret_cast<int64_t*>(ptr + summary_bytes);
    }

    void update(uint64_t i, int64_t delta) {
        if (i == fanout) return;
        assert(i < fanout);
        summary[i / segments] += delta;
        keys[i] += delta;
    }

    int64_t sum(uint64_t i) const {
        assert(i < fanout);

        uint64_t j = i / segments;
        uint64_t k = i % segments;
        uint64_t base = j * segments;

        // 1. prefix sum on the summary
        static int64_t summary_tmp[segments];
        summary_tmp[0] = 0;
        summary_tmp[1] = summary[0];
        summary_tmp[2] = summary[0] + summary[1];
        summary_tmp[3] = summary[0] + summary[1] + summary[2];
        if (j > 3) {
            summary_tmp[4] = summary[0] + summary[1] + summary[2] + summary[3];
            summary_tmp[5] =
                summary[0] + summary[1] + summary[2] + summary[3] + summary[4];
            summary_tmp[6] = summary[0] + summary[1] + summary[2] + summary[3] +
                             summary[4] + summary[5];
            summary_tmp[7] = summary[0] + summary[1] + summary[2] + summary[3] +
                             summary[4] + summary[5] + summary[6];
        }
        int64_t s1 = summary_tmp[j];

        // 2. prefix sum on a specific segment
        static int64_t keys_tmp[segments];
        int64_t* __keys = keys + base;
        keys_tmp[0] = __keys[0];
        keys_tmp[1] = __keys[0] + __keys[1];
        keys_tmp[2] = __keys[0] + __keys[1] + __keys[2];
        keys_tmp[3] = __keys[0] + __keys[1] + __keys[2] + __keys[3];
        if (k > 3) {
            keys_tmp[4] =
                __keys[0] + __keys[1] + __keys[2] + __keys[3] + __keys[4];
            keys_tmp[5] = __keys[0] + __keys[1] + __keys[2] + __keys[3] +
                          __keys[4] + __keys[5];
            keys_tmp[6] = __keys[0] + __keys[1] + __keys[2] + __keys[3] +
                          __keys[4] + __keys[5] + __keys[6];
            keys_tmp[7] = __keys[0] + __keys[1] + __keys[2] + __keys[3] +
                          __keys[4] + __keys[5] + __keys[6] + __keys[7];
        }
        int64_t s2 = keys_tmp[k];

        return s1 + s2;
    }

private:
    int64_t* summary;
    int64_t* keys;
};

}  // namespace psds