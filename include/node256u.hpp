#pragma once

#include <algorithm>
#include <cassert>

#include "immintrin.h"
#include "tables.hpp"

namespace psds {

struct node256u {
    static constexpr uint32_t degree = 256;
    static constexpr uint32_t U_size = 1;
    static constexpr uint32_t L_size = 8 * 4 + 4;
    static constexpr uint32_t B_size = 256;
    static constexpr uint32_t S_size = 256 * 4;
    static constexpr uint32_t size = U_size + L_size + B_size + S_size;

    static void build(int32_t const* input, uint8_t* out) {
        std::fill(out, out + size, 0);
        int32_t* L = reinterpret_cast<int32_t*>(out + U_size);
        int32_t* S = reinterpret_cast<int32_t*>(out + U_size + L_size + B_size);
        L[0] = 0;
        for (uint32_t i = 0; i != 8; ++i) {
            uint32_t base = i * 32;
            S[base] = input[base];
            for (uint32_t j = 1; j != 32; ++j) {
                S[base + j] = S[base + j - 1] + input[base + j];
            }
            L[i + 1] = L[i] + S[(i + 1) * 32 - 1];
        }
    }

    static std::string name() {
        return "node256u";
    }

    node256u(uint8_t* ptr) {
        U = ptr;
        ptr += U_size;
        L = reinterpret_cast<int32_t*>(ptr);
        ptr += L_size;
        B = reinterpret_cast<int8_t*>(ptr);
        ptr += B_size;
        S = reinterpret_cast<int32_t*>(ptr);
    }

    void update(uint32_t i, int8_t delta) {
        assert(i < 256);
        assert(delta == +1 or delta == -1);

        uint32_t j = i / 32;
        uint32_t k = i % 32;
        bool sign = delta >> 7;

        if (*U == 127) {
            for (uint32_t i = 0; i != 256; ++i) {
                S[i] += B[i];
                B[i] = 0;
            }
            *U = 0;
        }

        __m256i s1 =
            _mm256_load_si256((__m256i const*)tables::T_L + j + sign * 8);
        __m256i d1 = _mm256_loadu_si256((__m256i const*)(L + 1));
        __m256i r1 = _mm256_add_epi32(d1, s1);
        _mm256_storeu_si256((__m256i*)(L + 1), r1);

        __m256i s2 =
            _mm256_load_si256((__m256i const*)tables::T_B + k + sign * 32);
        __m256i d2 = _mm256_loadu_si256((__m256i const*)(B + j * 32));
        __m256i r2 = _mm256_add_epi8(d2, s2);
        _mm256_storeu_si256((__m256i*)(B + j * 32), r2);

        *U += 1;
    }

    int32_t sum(uint32_t i) const {
        assert(i < 256);
        return L[i / 32] + B[i] + S[i];
    }

private:
    uint8_t* U;
    int32_t* L;
    int8_t* B;
    int32_t* S;
};

}  // namespace psds