#pragma once

#include <cassert>

#include "immintrin.h"
#include "common.hpp"
#include "tables.hpp"

namespace psds {

struct node64u_restricted {
    static constexpr uint64_t fanout = 64;
    static constexpr uint64_t segments = 8;
    static constexpr uint64_t summary_buffer_bytes = segments * sizeof(int16_t);
    static constexpr uint64_t summary_bytes = segments * sizeof(int64_t);
    static constexpr uint64_t keys_buffer_bytes = fanout * sizeof(int16_t);
    static constexpr uint64_t keys_bytes = fanout * sizeof(int64_t);
    static constexpr uint64_t bytes = summary_buffer_bytes + summary_bytes +
                                      keys_buffer_bytes + keys_bytes + 1;

    node64u_restricted() {}  // do not initialize

    template <typename T>
    static void build(T const* input, uint8_t* out) {
        build_node_prefix_sums(input, out, fanout, segments, summary_bytes,
                               bytes);
        out[bytes - 1] = 255;  // initialize the number of updates
    }

    static std::string name() {
        return "node64u_restricted";
    }

    node64u_restricted(uint8_t* ptr) {
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
        uint64_t j = i / segments;
        uint64_t k = i % segments;

#ifdef DISABLE_AVX
        static constexpr uint64_t segment_size = fanout / segments;
        for (uint64_t z = j + 1; z != segments; ++z) summary_buffer[z] += delta;
        for (uint64_t z = k, base = j * segments; z != segment_size; ++z) {
            keys_buffer[base + z] += delta;
        }
#else
        __m128i upd = _mm_set1_epi16(delta);

        __m128i msk_j =
            _mm_load_si128((__m128i const*)tables::restricted::mask8_j + j);
        __m128i upd_j = _mm_and_si128(upd, msk_j);
        __m128i dst_summary_buffer =
            _mm_loadu_si128((__m128i const*)summary_buffer);
        __m128i res_summary_buffer = _mm_add_epi16(upd_j, dst_summary_buffer);
        _mm_storeu_si128((__m128i*)summary_buffer, res_summary_buffer);

        __m128i msk_k =
            _mm_load_si128((__m128i const*)tables::restricted::mask8_k + k);
        __m128i upd_k = _mm_and_si128(upd, msk_k);
        __m128i dst_keys_buffer =
            _mm_loadu_si128((__m128i const*)keys_buffer + j);
        __m128i res_keys_buffer = _mm_add_epi16(upd_k, dst_keys_buffer);
        _mm_storeu_si128((__m128i*)keys_buffer + j, res_keys_buffer);
#endif

        *updates += 1;
        if (*updates == 255) {
            for (uint64_t z = 0; z != segments; ++z) {
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
        uint64_t j = i / segments;
        int64_t s1 = summary_buffer[j];
        int64_t s2 = keys_buffer[i];
        return summary[j] + keys[i] + s1 + s2;
    }

private:
    int64_t* summary;
    int64_t* keys;

    int16_t* summary_buffer;
    int16_t* keys_buffer;
    uint8_t* updates;
};

}  // namespace psds