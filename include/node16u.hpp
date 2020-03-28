#pragma once

#include <cassert>

#include "immintrin.h"
#include "common.hpp"
#include "tables.hpp"

namespace psds {

struct node16u {
    static constexpr uint64_t fanout = 16;
    static constexpr uint64_t segments = 4;
    static constexpr uint64_t summary_bytes = (segments + 1) * sizeof(int64_t);
    static constexpr uint64_t bytes = summary_bytes + fanout * sizeof(int64_t);

    template <typename T>
    static void build(T const* input, uint8_t* out) {
        build_node_prefix_sums(input, out, segments, summary_bytes, bytes);
    }

    static std::string name() {
        return "node16u";
    }

    node16u(uint8_t* ptr) {
        summary = reinterpret_cast<int64_t*>(ptr);
        keys = reinterpret_cast<int64_t*>(ptr + summary_bytes);
    }

    void update(uint64_t i, int64_t delta) {
        assert(i < fanout);

        uint64_t j = i / segments;
        uint64_t k = i % segments;

#ifdef DISABLE_AVX
        for (uint64_t z = j + 1; z != segments + 1; ++z) summary[z] += delta;
        for (uint64_t z = k, base = j * segments; z != segments; ++z) {
            keys[base + z] += delta;
        }
#else
        __m256i upd = _mm256_set1_epi64x(delta);

        __m256i msk_j = _mm256_load_si256((__m256i const*)tables::mask_j + j);
        __m256i upd_j = _mm256_and_si256(upd, msk_j);

        __m256i dst = _mm256_loadu_si256((__m256i const*)summary);
        __m256i res = _mm256_add_epi64(upd_j, dst);
        _mm256_storeu_si256((__m256i*)summary, res);

        __m256i msk_k = _mm256_load_si256((__m256i const*)tables::mask_k + k);
        __m256i upd_k = _mm256_and_si256(upd, msk_k);

        __m256i dst_ =
            _mm256_loadu_si256((__m256i const*)(keys + j * segments));
        __m256i res_ = _mm256_add_epi64(upd_k, dst_);
        _mm256_storeu_si256((__m256i*)(keys + j * segments), res_);
#endif
    }

    int64_t sum(uint64_t i) const {
        assert(i < fanout);
        return summary[i / segments] + keys[i];
    }

private:
    int64_t* summary;
    int64_t* keys;
};

}  // namespace psds