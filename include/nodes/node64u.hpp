#pragma once

#include <cassert>

#include "immintrin.h"
#include "common.hpp"
#include "tables.hpp"

namespace psds {

struct node64u {
    static constexpr uint64_t fanout = 64;
    static constexpr uint64_t segment_size = 8;
    static constexpr uint64_t summary_bytes = segment_size * sizeof(int64_t);
    static constexpr uint64_t bytes = summary_bytes + fanout * sizeof(int64_t);

    node64u() {}  // do not initialize

    template <typename T>
    static void build(T const* input, uint8_t* out) {
        build_node_prefix_sums(input, out, segment_size, summary_bytes, bytes);
    }

    static std::string name() {
        return "node64u";
    }

    node64u(uint8_t* ptr) {
        at(ptr);
    }

    inline void at(uint8_t* ptr) {
        summary = reinterpret_cast<int64_t*>(ptr);
        keys = reinterpret_cast<int64_t*>(ptr + summary_bytes);
    }

    void update(uint64_t i, int64_t delta) {
        if (i == fanout) return;

        assert(i < fanout);
        uint64_t j = i / segment_size;
        uint64_t k = i % segment_size;

#ifdef DISABLE_AVX
        static constexpr uint64_t segment_size = fanout / segment_size;
        for (uint64_t z = j + 1; z != segment_size; ++z) summary[z] += delta;
        for (uint64_t z = k, base = j * segment_size; z != segment_size; ++z) {
            keys[base + z] += delta;
        }
#else
        __m256i upd = _mm256_set1_epi64x(delta);

        __m256i msk_j0 = _mm256_load_si256(
            (__m256i const*)tables::unrestricted::mask8_j + 2 * j + 0);
        __m256i msk_j1 = _mm256_load_si256(
            (__m256i const*)tables::unrestricted::mask8_j + 2 * j + 1);
        __m256i upd_j0 = _mm256_and_si256(upd, msk_j0);
        __m256i upd_j1 = _mm256_and_si256(upd, msk_j1);

        __m256i dst_summary0 = _mm256_loadu_si256((__m256i const*)summary + 0);
        __m256i dst_summary1 = _mm256_loadu_si256((__m256i const*)summary + 1);

        __m256i res_summary0 = _mm256_add_epi64(upd_j0, dst_summary0);
        __m256i res_summary1 = _mm256_add_epi64(upd_j1, dst_summary1);
        _mm256_storeu_si256((__m256i*)summary + 0, res_summary0);
        _mm256_storeu_si256((__m256i*)summary + 1, res_summary1);

        __m256i msk_k0 = _mm256_load_si256(
            (__m256i const*)tables::unrestricted::mask8_k + 2 * k + 0);
        __m256i msk_k1 = _mm256_load_si256(
            (__m256i const*)tables::unrestricted::mask8_k + 2 * k + 1);

        __m256i upd_k0 = _mm256_and_si256(upd, msk_k0);
        __m256i upd_k1 = _mm256_and_si256(upd, msk_k1);

        __m256i dst_keys0 =
            _mm256_loadu_si256((__m256i const*)(keys + j * segment_size) + 0);
        __m256i dst_keys1 =
            _mm256_loadu_si256((__m256i const*)(keys + j * segment_size) + 1);

        __m256i res_keys0 = _mm256_add_epi64(upd_k0, dst_keys0);
        __m256i res_keys1 = _mm256_add_epi64(upd_k1, dst_keys1);
        _mm256_storeu_si256((__m256i*)(keys + j * segment_size) + 0, res_keys0);
        _mm256_storeu_si256((__m256i*)(keys + j * segment_size) + 1, res_keys1);
#endif
    }

    int64_t sum(uint64_t i) const {
        assert(i < fanout);
        return summary[i / segment_size] + keys[i];
    }

private:
    int64_t* summary;
    int64_t* keys;
};

}  // namespace psds