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

static constexpr uint32_t min_log2 = 0;
static constexpr uint32_t max_log2 = 20;

}  // namespace testing
}  // namespace psds