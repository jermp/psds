#pragma once

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../external/doctest/doctest/doctest.h"

#include <iostream>
#include <vector>

#include "../external/essentials/include/essentials.hpp"
#include "types.hpp"

using namespace psds;

namespace psds {
namespace testing {
static constexpr uint32_t min_log2 = 8;
static constexpr uint32_t max_log2 = 24;
static_assert(min_log2 >= 8, "min_log2 must be 8 at least");
}  // namespace testing
}  // namespace psds