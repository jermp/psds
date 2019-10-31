#include "test_common.hpp"
#include "test_node.hpp"

// struct __node256u2 {
//     static constexpr uint32_t degree = 256;
//     static constexpr uint32_t U_size = (degree / 32 + 1) * 4;
//     static constexpr uint32_t S_size = degree * 4;
//     static constexpr uint32_t size = S_size + U_size;

//     static void build(int32_t const* input, uint8_t* out) {
//         std::fill(out, out + size, 0);
//         int32_t* U = reinterpret_cast<int32_t*>(out);
//         int32_t* S = reinterpret_cast<int32_t*>(out + U_size);
//         U[0] = 0;
//         for (uint32_t i = 0; i != 8; ++i) {
//             uint32_t base = i * 32;
//             S[base] = input[base];
//             for (uint32_t j = 1; j != 32; ++j) {
//                 S[base + j] = S[base + j - 1] + input[base + j];
//             }
//             U[i + 1] = U[i] + S[(i + 1) * 32 - 1];
//         }
//     }

//     static std::string name() {
//         return "__node256u2";
//     }

//     __node256u2(uint8_t* ptr) {
//         U = reinterpret_cast<int32_t*>(ptr);
//         S = reinterpret_cast<int32_t*>(ptr + U_size);
//     }

//     void update(uint32_t i, int8_t delta) {
//         assert(i < degree);
//         assert(delta == +1 or delta == -1);

//         uint32_t j = i / 32;
//         uint32_t k = i % 32;

// #ifdef DISABLE_AVX
//         // for (uint32_t z = j + 1; z != 8 + 1; ++z) L[z] += delta;
//         // for (uint32_t z = k, base = j * 32; z != 32; ++z) B[base + z] +=
//         // delta;
// #else
//         bool sign = delta >> 7;  // is it faster doing & (1 << 31) and
//                                  // comparison agains 1 << 31 ?

//         __m256i s1 =
//             _mm256_load_si256((__m256i const*)tables::T_L + j + sign * 8);
//         __m256i d1 = _mm256_loadu_si256((__m256i const*)(U + 1));
//         __m256i r1 = _mm256_add_epi32(d1, s1);
//         _mm256_storeu_si256((__m256i*)(U + 1), r1);

//         uint32_t jj = k / 8;
//         uint32_t kk = k % 8;
//         __m256i s2 =
//             _mm256_load_si256((__m256i const*)tables::T_L + kk + sign * 8);
//         __m256i d2 = _mm256_loadu_si256((__m256i const*)(S + j * 32 + jj *
//         8));
//         __m256i r2 = _mm256_add_epi32(d2, s2);
//         _mm256_storeu_si256((__m256i*)(S + j * 32 + jj * 8), r2);

//         ++jj;
//         if (jj != 4) {
//             __m256i all;
//             if (sign) {  // negative
//                 all = _mm256_set_epi32(-1, -1, -1, -1, -1, -1, -1, -1);
//             } else {  // positive
//                 all = _mm256_set_epi32(+1, +1, +1, +1, +1, +1, +1, +1);
//             }

//             for (; jj != 4; ++jj) {
//                 __m256i d =
//                     _mm256_loadu_si256((__m256i const*)(S + j * 32 + jj *
//                     8));
//                 __m256i r = _mm256_add_epi32(d, all);
//                 _mm256_storeu_si256((__m256i*)(S + j * 32 + jj * 8), r);
//             }
//         }
// #endif
//     }

//     int32_t sum(uint32_t i) const {
//         assert(i < degree);
//         assert(U[0] == 0);
//         return U[i / 32] + S[i];
//     }

// private:
//     int32_t* U;
//     int32_t* S;
// };

TEST_CASE("test node256u2") {
    psds::testing::test_node<node256u2>();
}
