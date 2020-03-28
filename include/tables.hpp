#pragma once

#if defined(_MSC_VER)
#define ALIGNED(x) __declspec(align(x))
#else
#if defined(__GNUC__)
#define ALIGNED(x) __attribute__((aligned(x)))
#endif
#endif

namespace psds {
namespace tables {

static const uint64_t all1 = -1;

static const uint64_t mask_j[4 * 4] ALIGNED(32) = {
    0, all1, all1, all1, 0, 0, all1, all1, 0, 0, 0, all1, 0, 0, 0, 0};

static const uint64_t mask_k[4 * 4] ALIGNED(32) = {
    all1, all1, all1, all1, 0, all1, all1, all1,
    0,    0,    all1, all1, 0, 0,    0,    all1};

}  // namespace tables
}  // namespace psds