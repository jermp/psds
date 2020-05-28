#pragma once

#include <cassert>

#include "immintrin.h"
#include "common.hpp"
#include "tables.hpp"

namespace psds {

struct node256_restricted {
    static constexpr uint64_t fanout = 256;
    static constexpr uint64_t segment_size = 16;
    static constexpr uint64_t summary_buffer_bytes =
        segment_size * sizeof(int16_t);
    static constexpr uint64_t summary_bytes = segment_size * sizeof(int64_t);
    static constexpr uint64_t keys_buffer_bytes = fanout * sizeof(int16_t);
    static constexpr uint64_t keys_bytes = fanout * sizeof(int64_t);
    static constexpr uint64_t bytes = summary_buffer_bytes + summary_bytes +
                                      keys_buffer_bytes + keys_bytes + 1;

    node256_restricted() {}  // do not initialize

    template <typename T>
    static void build(T const* input, uint8_t* out) {
        build_node_prefix_sums(input, out, segment_size, bytes);
        out[bytes - 1] = 255;  // initialize the number of updates
    }

    static std::string name() {
        return "node256_restricted";
    }

    node256_restricted(uint8_t* ptr) {
        at(ptr);
    }

    inline void at(uint8_t* ptr) {
        summary = reinterpret_cast<int64_t*>(ptr);
        keys = reinterpret_cast<int64_t*>(ptr + summary_bytes);
        summary_buffer =
            reinterpret_cast<int16_t*>(ptr + summary_bytes + keys_bytes);
        keys_buffer = reinterpret_cast<int16_t*>(
            ptr + summary_bytes + keys_bytes + summary_buffer_bytes);
        updates = ptr + bytes - 1;
    }

    void update(uint64_t i, int8_t delta) {
        if (i == fanout) return;

        assert(i < fanout);
        uint64_t j = i / segment_size;
        uint64_t k = i % segment_size;

#ifdef DISABLE_AVX
        for (uint64_t z = j + 1; z != segment_size; ++z) {
            summary_buffer[z] += delta;
        }
        for (uint64_t z = k, base = j * segment_size; z != segment_size; ++z) {
            keys_buffer[base + z] += delta;
        }
#else
        __m256i upd = _mm256_set1_epi16(delta);

        __m256i msk_j = _mm256_load_si256(
            (__m256i const*)(tables::restricted::mask16 + 16) + j);
        __m256i upd_j = _mm256_and_si256(upd, msk_j);
        __m256i dst_summary_buffer =
            _mm256_loadu_si256((__m256i const*)summary_buffer);
        __m256i res_summary_buffer =
            _mm256_add_epi16(upd_j, dst_summary_buffer);
        _mm256_storeu_si256((__m256i*)summary_buffer, res_summary_buffer);

        __m256i msk_k =
            _mm256_load_si256((__m256i const*)tables::restricted::mask16 + k);
        __m256i upd_k = _mm256_and_si256(upd, msk_k);
        __m256i dst_keys_buffer =
            _mm256_loadu_si256((__m256i const*)keys_buffer + j);
        __m256i res_keys_buffer = _mm256_add_epi16(upd_k, dst_keys_buffer);
        _mm256_storeu_si256((__m256i*)keys_buffer + j, res_keys_buffer);
#endif

        *updates += 1;
        if (*updates == 255) {
            for (uint64_t z = 0; z != segment_size; ++z) {
                summary[z] += summary_buffer[z];
                summary_buffer[z] = 0;
            }
            for (uint64_t z = 0; z != fanout; ++z) {
                keys[z] += keys_buffer[z];
                keys_buffer[z] = 0;
            }
        }
    }

    int64_t sum(uint64_t i) const {
        assert(i < fanout);
        uint64_t j = i / segment_size;
        int64_t s1 = summary_buffer[j];
        int64_t s2 = keys_buffer[i];
        return summary[j] + keys[i] + s1 + s2;
    }

    int64_t back() const {
        return (keys[fanout - 1] - keys[fanout - 2]) +
               (keys_buffer[fanout - 1] - keys_buffer[fanout - 2]);
    }

    void update_back(int64_t delta) {
        keys[fanout - 1] += delta;
        *updates += 1;
        if (*updates == 255) {
            for (uint64_t z = 0; z != segment_size; ++z) {
                summary[z] += summary_buffer[z];
                summary_buffer[z] = 0;
            }
            for (uint64_t z = 0; z != fanout; ++z) {
                keys[z] += keys_buffer[z];
                keys_buffer[z] = 0;
            }
        }
    }

private:
    int64_t* summary;
    int64_t* keys;
    int16_t* summary_buffer;
    int16_t* keys_buffer;
    uint8_t* updates;
};

}  // namespace psds