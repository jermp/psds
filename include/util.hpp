#pragma once

namespace psds::util {

constexpr size_t ceil_log2(size_t n) {
    return (n < 2) ? 1 : 1 + ceil_log2(n / 2);
}

constexpr size_t floor_log2(size_t n) {
    return (n == 1) ? 0 : 1 + floor_log2(n / 2);
}

constexpr double const_ceil(double val) {
    const auto val_int = (int64_t)val;
    const double fval_int = (double)val_int;
    return (val < double(0) ? fval_int
                            : (val == fval_int ? val : fval_int + double(1)));
}

constexpr uint32_t ceil_log(uint32_t base, uint32_t n) {
    assert(base > 0);
    assert((base & (base - 1)) == 0);  // base must be a power of 2
    return const_ceil(static_cast<double>(ceil_log2(n)) /
                      static_cast<double>(floor_log2(base)));
}

}  // namespace psds::util