#pragma once

#include "tree_epi32.hpp"
#include "node256u.hpp"
#include "node256s.hpp"
#include "node64u.hpp"
#include "node64s.hpp"
#include "segment_tree_vanilla.hpp"
#include "fenwick_tree.hpp"
#include "truncated_fenwick_tree_epi32.hpp"
#include "blocked_fenwick_tree_epi32.hpp"

namespace psds {

typedef segment_tree_vanilla<int32_t> segment_tree_type;
typedef fenwick_tree<int32_t> fenwick_tree_type;

}  // namespace psds