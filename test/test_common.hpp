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

static constexpr uint32_t logs[] = {8,  9,  10, 11, 12, 13, 14, 15, 16,
                                    17, 18, 19, 20, 21, 22, 23, 24};

}  // namespace testing
}  // namespace psds