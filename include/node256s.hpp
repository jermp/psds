#pragma once

#include <cassert>
#include <limits>

#include "immintrin.h"
#include "tables.hpp"

namespace psds {

struct node256s {
    static constexpr int8_t max_update = std::numeric_limits<int8_t>::max();
    static constexpr int8_t min_update = std::numeric_limits<int8_t>::min();
    static constexpr uint32_t degree = 256;
    static constexpr uint32_t L_size = 8 * 2;
    static constexpr uint32_t B_size = 256;
    static constexpr uint32_t S_size = 256 * 4;
    static constexpr uint32_t size = L_size + B_size + S_size;

    static void build(int32_t const* input, uint8_t* out) {
        std::fill(out, out + size, 0);
        int32_t* S = reinterpret_cast<int32_t*>(out + L_size + B_size);
        S[0] = input[0];
        for (uint32_t i = 1; i != 256; ++i) S[i] = S[i - 1] + input[i];
    }

    node256s(uint8_t* ptr) {
        L = reinterpret_cast<int16_t*>(ptr);
        ptr += L_size;
        B = reinterpret_cast<int8_t*>(ptr);
        ptr += B_size;
        S = reinterpret_cast<int32_t*>(ptr);
    }

    static std::string name() {
        return "node256s";
    }

    void update(uint32_t i, int8_t delta) {
        assert(i < 256);
        assert(delta == +1 or delta == -1);

        if (B[i] == max_update or B[i] == min_update) {
            int32_t sum = 0;
            for (uint32_t i = 0; i != 256; ++i) {
                sum += B[i];
                S[i] += sum;
                B[i] = 0;
            }
            for (uint32_t i = 0; i != 8; ++i) L[i] = 0;
        }

        L[i / 32] += delta;
        B[i] += delta;
    }

    int32_t sum(uint32_t i) const {
        assert(i < 256);
        int32_t s = S[i];

        uint32_t j = i / 32;
        uint32_t k = i % 32;

        __m128i upd1 = _mm_lddqu_si128((__m128i*)L);
        __m128i mask1 = _mm_load_si128((__m128i const*)(tables::masks + j * 8));
        upd1 = _mm_and_si128(upd1, mask1);
        upd1 = _mm_madd_epi16(upd1, _mm_set1_epi16(1));
        upd1 = _mm_add_epi32(upd1, _mm_srli_si128(upd1, 8));
        upd1 = _mm_add_epi32(upd1, _mm_srli_si128(upd1, 4));

        s += _mm_cvtsi128_si32(upd1);

        __m256i upd2 = _mm256_lddqu_si256((__m256i*)(B + (i & 0xe0)));
        __m256i mask2 =
            _mm256_load_si256((__m256i const*)(tables::masks + (1 + k) * 4));
        upd2 = _mm256_and_si256(upd2, mask2);
        __m256i upd2_l =
            _mm256_cvtepi8_epi16(_mm256_extracti128_si256(upd2, 0));
        __m256i upd2_h =
            _mm256_cvtepi8_epi16(_mm256_extracti128_si256(upd2, 1));
        upd2 = _mm256_add_epi16(upd2_h, upd2_l);
        upd2 = _mm256_madd_epi16(upd2, _mm256_set1_epi16(1));
        upd2 = _mm256_add_epi32(upd2, _mm256_bsrli_epi128(upd2, 8));
        upd2 = _mm256_add_epi32(upd2, _mm256_bsrli_epi128(upd2, 4));

        s += _mm256_extract_epi32(upd2, 0) + _mm256_extract_epi32(upd2, 4);

        return s;
    }

private:
    int16_t* L;
    int8_t* B;
    int32_t* S;
};

}  // namespace psds