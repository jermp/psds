#pragma once

#if defined(_MSC_VER)
#define ALIGNED(x) __declspec(align(x))
#else
#if defined(__GNUC__)
#define ALIGNED(x) __attribute__((aligned(x)))
#endif
#endif

#ifdef AVX_512
#define ALIGNEMENT ALIGNED(64)
#else
#define ALIGNEMENT ALIGNED(32)
#endif

namespace psds::tables::unrestricted {
static const uint64_t all1 = uint64_t(-1);

static const uint64_t mask8[(8 + 1) * 8] ALIGNEMENT = {
    all1, all1, all1, all1, all1, all1, all1, all1, 0,    all1, all1, all1,
    all1, all1, all1, all1, 0,    0,    all1, all1, all1, all1, all1, all1,
    0,    0,    0,    all1, all1, all1, all1, all1, 0,    0,    0,    0,
    all1, all1, all1, all1, 0,    0,    0,    0,    0,    all1, all1, all1,
    0,    0,    0,    0,    0,    0,    all1, all1, 0,    0,    0,    0,
    0,    0,    0,    all1, 0,    0,    0,    0,    0,    0,    0,    0};

static const uint64_t mask16[(16 + 1) * 16] ALIGNEMENT = {
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, 0,    all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, 0,    0,    all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    0,    0,    0,    all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, 0,    0,    0,    0,    all1, all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, 0,    0,    0,    0,
    0,    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    0,    0,    0,    0,    0,    0,    all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, 0,    0,    0,    0,    0,    0,    0,    all1,
    all1, all1, all1, all1, all1, all1, all1, all1, 0,    0,    0,    0,
    0,    0,    0,    0,    all1, all1, all1, all1, all1, all1, all1, all1,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    all1, all1, all1,
    all1, all1, all1, all1, 0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    all1, all1, all1, all1, all1, all1, 0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    all1, all1, all1, all1, all1,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    all1, all1, all1, all1, 0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    all1, all1, all1, 0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    all1, all1,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    all1, 0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0};

}  // namespace psds::tables::unrestricted

namespace psds::tables::restricted {
static const uint16_t all1 = uint16_t(-1);

static const uint16_t mask8[(8 + 1) * 8] ALIGNEMENT = {
    all1, all1, all1, all1, all1, all1, all1, all1, 0,    all1, all1, all1,
    all1, all1, all1, all1, 0,    0,    all1, all1, all1, all1, all1, all1,
    0,    0,    0,    all1, all1, all1, all1, all1, 0,    0,    0,    0,
    all1, all1, all1, all1, 0,    0,    0,    0,    0,    all1, all1, all1,
    0,    0,    0,    0,    0,    0,    all1, all1, 0,    0,    0,    0,
    0,    0,    0,    all1, 0,    0,    0,    0,    0,    0,    0,    0};

static const uint16_t mask16[(16 + 1) * 16] ALIGNEMENT = {
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, 0,    all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, 0,    0,    all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    0,    0,    0,    all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, 0,    0,    0,    0,    all1, all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, 0,    0,    0,    0,
    0,    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    0,    0,    0,    0,    0,    0,    all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, 0,    0,    0,    0,    0,    0,    0,    all1,
    all1, all1, all1, all1, all1, all1, all1, all1, 0,    0,    0,    0,
    0,    0,    0,    0,    all1, all1, all1, all1, all1, all1, all1, all1,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    all1, all1, all1,
    all1, all1, all1, all1, 0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    all1, all1, all1, all1, all1, all1, 0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    all1, all1, all1, all1, all1,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    all1, all1, all1, all1, 0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    all1, all1, all1, 0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    all1, all1,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    all1, 0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0};

static const uint16_t mask32[(32 + 1) * 32] ALIGNEMENT = {
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, 0,    all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, 0,    0,    all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    0,    0,    0,    all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, 0,    0,    0,    0,
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, 0,    0,    0,    0,    0,    all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    0,    0,    0,    0,    0,    0,    all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, 0,    0,    0,    0,
    0,    0,    0,    all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, 0,    0,    0,    0,    0,    0,    0,    0,
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, 0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, 0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, 0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, 0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, 0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, 0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, 0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    all1, all1, all1, all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, 0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1, all1,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    all1, all1, all1,
    all1, all1, all1, all1, all1, all1, all1, all1, 0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    all1, all1, all1, all1, all1, all1,
    all1, all1, all1, all1, 0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    all1, all1, all1, all1, all1, all1, all1, all1, all1,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    all1, all1, all1, all1, all1, all1, all1, all1, 0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    all1, all1, all1,
    all1, all1, all1, all1, 0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    all1, all1, all1, all1, all1, all1,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    all1, all1, all1, all1, all1, 0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    all1, all1, all1, all1, 0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    all1, all1, all1,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    all1, all1, 0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    all1, 0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0};

}  // namespace psds::tables::restricted