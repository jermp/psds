#pragma once

#include <algorithm>
#include <cassert>

#include "immintrin.h"
#include "tables.hpp"

namespace psds {

struct node64s {
    static constexpr uint32_t degree = 64;
    static constexpr uint32_t U_size = 8 * 4 + 4;
    static constexpr uint32_t block_size = 8 * 4;
    static constexpr uint32_t size = U_size + 8 * block_size;

    struct builder {
        void build(int32_t const* input, uint8_t* out) {
            std::fill(out, out + size, 0);
            int32_t* U = reinterpret_cast<int32_t*>(out);
            int32_t* S = reinterpret_cast<int32_t*>(out + U_size);
            U[0] = 0;
            for (uint32_t i = 0; i != 8; ++i) {
                uint32_t base = i * 8;
                int32_t sum = 0;
                for (uint32_t j = 0; j != 8; ++j) {
                    S[base + j] = input[base + j];
                    sum += input[base + j];
                }
                U[i + 1] = U[i] + sum;
            }
        }
    };

    node64s(uint8_t* ptr) {
        U = reinterpret_cast<int32_t*>(ptr);
        S = reinterpret_cast<int32_t*>(ptr + U_size);
    }

    static std::string name() {
        return "node64s";
    }

    void update(uint32_t i, int8_t delta) {
        assert(i < 64);
        uint32_t j = i / 8;
        bool sign = delta >> 7;
        __m256i s1 = _mm256_load_si256((__m256i const*)T_L + j + sign * 8);
        __m256i d1 = _mm256_loadu_si256((__m256i const*)(U + 1));
        __m256i r1 = _mm256_add_epi32(d1, s1);
        _mm256_storeu_si256((__m256i*)(U + 1), r1);
        S[i] += delta;
    }

    int32_t sum(uint32_t i) const {
        assert(i < 64);
        uint32_t j = i / 8;
        uint32_t k = i % 8;
        assert(U[0] == 0);
        int32_t s = U[j];

#if __linux__
        static int32_t tmp[4];
#endif
        __m128i r1 = _mm_loadu_si128((__m128i const*)(S + j * 8));
        r1 = _mm_add_epi32(r1, _mm_slli_si128(r1, 4));
        r1 = _mm_add_epi32(r1, _mm_slli_si128(r1, 8));

        if (k < 4) {
#if __linux__
            // NOTE: gcc does not support
            // _mm_extract_epi32 with variable index,
            // thus we need to store :(
            _mm_store_si128((__m128i*)(tmp), r1);
            s += tmp[k];
#else
            s += _mm_extract_epi32(r1, k);
#endif
        } else {
            int32_t last = _mm_extract_epi32(r1, 3);
            __m128i r2 = _mm_loadu_si128((__m128i const*)(S + j * 8 + 4));
            r2 = _mm_add_epi32(r2, _mm_slli_si128(r2, 4));
            r2 = _mm_add_epi32(r2, _mm_slli_si128(r2, 8));
            r2 = _mm_add_epi32(r2, _mm_set1_epi32(last));

#if __linux__
            _mm_store_si128((__m128i*)(tmp), r2);
            s += tmp[k - 4];
#else
            s += _mm_extract_epi32(r2, k - 4);
#endif
        }

        return s;
    }

private:
    int32_t* U;
    int32_t* S;
};

}  // namespace psds