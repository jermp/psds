#pragma once

#include <cassert>

#include "immintrin.h"
#include "tables.hpp"

namespace psds {

struct node64u {
    static constexpr uint32_t degree = 64;
    static constexpr uint32_t U_size = 8 * 4 + 4;
    static constexpr uint32_t block_size = 8 * 4;
    static constexpr uint32_t size = U_size + 8 * block_size;

    static void build(int32_t const* input, uint8_t* out) {
        std::fill(out, out + size, 0);
        int32_t* U = reinterpret_cast<int32_t*>(out);
        int32_t* S = reinterpret_cast<int32_t*>(out + U_size);
        U[0] = 0;
        for (uint32_t i = 0; i != 8; ++i) {
            uint32_t base = i * 8;
            S[base] = input[base];
            for (uint32_t j = 1; j != 8; ++j) {
                S[base + j] = S[base + j - 1] + input[base + j];
            }
            U[i + 1] = U[i] + S[(i + 1) * 8 - 1];
        }
    }

    static std::string name() {
        return "node64u";
    }

    node64u(uint8_t* ptr) {
        U = reinterpret_cast<int32_t*>(ptr);
        S = reinterpret_cast<int32_t*>(ptr + U_size);
    }

    void update(uint32_t i, int8_t delta) {
        assert(i < 64);
        assert(delta == +1 or delta == -1);

        uint32_t j = i / 8;
        uint32_t k = i % 8;
        bool sign = delta >> 7;

        __m256i s1 =
            _mm256_load_si256((__m256i const*)tables::T_L + j + sign * 8);
        __m256i d1 = _mm256_loadu_si256((__m256i const*)(U + 1));
        __m256i r1 = _mm256_add_epi32(d1, s1);
        _mm256_storeu_si256((__m256i*)(U + 1), r1);

        __m256i s2 =
            _mm256_load_si256((__m256i const*)tables::T_L + k + sign * 8);
        __m256i d2 = _mm256_loadu_si256((__m256i const*)(S + j * 8));
        __m256i r2 = _mm256_add_epi32(d2, s2);
        _mm256_storeu_si256((__m256i*)(S + j * 8), r2);
    }

    int32_t sum(uint32_t i) const {
        assert(i < 64);
        return U[i / 8] + S[i];
    }

private:
    int32_t* U;
    int32_t* S;
};

}  // namespace psds