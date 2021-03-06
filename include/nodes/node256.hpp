#pragma once

#include <cassert>

#include "immintrin.h"
#include "common.hpp"
#include "tables.hpp"

namespace psds {

struct node256 {
    static constexpr uint64_t fanout = 256;
    static constexpr uint64_t segment_size = 16;
    static constexpr uint64_t bytes = (fanout + segment_size) * sizeof(int64_t);

    node256() {}  // do not initialize

    template <typename T>
    static void build(T const* input, uint8_t* out) {
        build_node_prefix_sums(input, out, segment_size, bytes);
    }

    static std::string name() {
        return "node256";
    }

    node256(uint8_t* ptr) {
        at(ptr);
    }

    inline void at(uint8_t* ptr) {
        summary = reinterpret_cast<int64_t*>(ptr);
        keys = reinterpret_cast<int64_t*>(ptr + segment_size * sizeof(int64_t));
    }

    void update(uint64_t i, int64_t delta) {
        if (i == fanout) return;

        assert(i < fanout);
        uint64_t j = i / segment_size;
        uint64_t k = i % segment_size;

#ifdef DISABLE_AVX
        for (uint64_t z = j + 1; z != segment_size; ++z) summary[z] += delta;
        for (uint64_t z = k, base = j * segment_size; z != segment_size; ++z) {
            keys[base + z] += delta;
        }
#else
        __m256i upd = _mm256_set1_epi64x(delta);

        __m256i msk_j0 = _mm256_load_si256(
            (__m256i const*)(tables::unrestricted::mask16 + 16) + 4 * j + 0);
        __m256i msk_j1 = _mm256_load_si256(
            (__m256i const*)(tables::unrestricted::mask16 + 16) + 4 * j + 1);
        __m256i msk_j2 = _mm256_load_si256(
            (__m256i const*)(tables::unrestricted::mask16 + 16) + 4 * j + 2);
        __m256i msk_j3 = _mm256_load_si256(
            (__m256i const*)(tables::unrestricted::mask16 + 16) + 4 * j + 3);
        __m256i upd_j0 = _mm256_and_si256(upd, msk_j0);
        __m256i upd_j1 = _mm256_and_si256(upd, msk_j1);
        __m256i upd_j2 = _mm256_and_si256(upd, msk_j2);
        __m256i upd_j3 = _mm256_and_si256(upd, msk_j3);

        __m256i dst_summary0 = _mm256_loadu_si256((__m256i const*)summary + 0);
        __m256i dst_summary1 = _mm256_loadu_si256((__m256i const*)summary + 1);
        __m256i dst_summary2 = _mm256_loadu_si256((__m256i const*)summary + 2);
        __m256i dst_summary3 = _mm256_loadu_si256((__m256i const*)summary + 3);

        __m256i res_summary0 = _mm256_add_epi64(upd_j0, dst_summary0);
        __m256i res_summary1 = _mm256_add_epi64(upd_j1, dst_summary1);
        __m256i res_summary2 = _mm256_add_epi64(upd_j2, dst_summary2);
        __m256i res_summary3 = _mm256_add_epi64(upd_j3, dst_summary3);
        _mm256_storeu_si256((__m256i*)summary + 0, res_summary0);
        _mm256_storeu_si256((__m256i*)summary + 1, res_summary1);
        _mm256_storeu_si256((__m256i*)summary + 2, res_summary2);
        _mm256_storeu_si256((__m256i*)summary + 3, res_summary3);

        __m256i msk_k0 = _mm256_load_si256(
            (__m256i const*)tables::unrestricted::mask16 + 4 * k + 0);
        __m256i msk_k1 = _mm256_load_si256(
            (__m256i const*)tables::unrestricted::mask16 + 4 * k + 1);
        __m256i msk_k2 = _mm256_load_si256(
            (__m256i const*)tables::unrestricted::mask16 + 4 * k + 2);
        __m256i msk_k3 = _mm256_load_si256(
            (__m256i const*)tables::unrestricted::mask16 + 4 * k + 3);
        __m256i upd_k0 = _mm256_and_si256(upd, msk_k0);
        __m256i upd_k1 = _mm256_and_si256(upd, msk_k1);
        __m256i upd_k2 = _mm256_and_si256(upd, msk_k2);
        __m256i upd_k3 = _mm256_and_si256(upd, msk_k3);

        __m256i dst_keys0 =
            _mm256_loadu_si256((__m256i const*)(keys + j * segment_size) + 0);
        __m256i dst_keys1 =
            _mm256_loadu_si256((__m256i const*)(keys + j * segment_size) + 1);
        __m256i dst_keys2 =
            _mm256_loadu_si256((__m256i const*)(keys + j * segment_size) + 2);
        __m256i dst_keys3 =
            _mm256_loadu_si256((__m256i const*)(keys + j * segment_size) + 3);

        __m256i res_keys0 = _mm256_add_epi64(upd_k0, dst_keys0);
        __m256i res_keys1 = _mm256_add_epi64(upd_k1, dst_keys1);
        __m256i res_keys2 = _mm256_add_epi64(upd_k2, dst_keys2);
        __m256i res_keys3 = _mm256_add_epi64(upd_k3, dst_keys3);

        _mm256_storeu_si256((__m256i*)(keys + j * segment_size) + 0, res_keys0);
        _mm256_storeu_si256((__m256i*)(keys + j * segment_size) + 1, res_keys1);
        _mm256_storeu_si256((__m256i*)(keys + j * segment_size) + 2, res_keys2);
        _mm256_storeu_si256((__m256i*)(keys + j * segment_size) + 3, res_keys3);
#endif
    }

    int64_t sum(uint64_t i) const {
        assert(i < fanout);
        return summary[i / segment_size] + keys[i];
    }

    int64_t back() const {
        return keys[fanout - 1] - keys[fanout - 2];
    }

    void update_back(int64_t delta) {
        keys[fanout - 1] += delta;
    }

private:
    int64_t* summary;
    int64_t* keys;
};

}  // namespace psds