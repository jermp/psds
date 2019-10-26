#pragma once

#include "tree_epi32.hpp"

#include "node256u.hpp"
#include "node256s.hpp"
#include "node64u.hpp"
#include "node64s.hpp"

#include "competitor/segment_tree_vanilla.hpp"
#include "competitor/fenwick_tree.hpp"

namespace psds {

// typedef tree_epi32<node256u> tree256u;
// typedef tree_epi32<node256s> tree256s;
// typedef tree_epi32<node64u> tree64u;
// typedef tree_epi32<node64s> tree64s;

typedef segment_tree_vanilla<int32_t> segment_tree_type;
typedef fenwick_tree<int32_t> fenwick_tree_type;

}  // namespace psds