#pragma once

#include <iostream>
#include <random>
#include <fstream>

#include "../external/essentials/include/essentials.hpp"
#include "types.hpp"

namespace psds {
namespace benchmarking {

static constexpr uint32_t num_queries = 1000000;
static constexpr unsigned value_seed = 13;
static constexpr unsigned query_seed = 71;

static constexpr uint32_t logs[] = {8,  9,  10, 11, 12, 13, 14, 15,
                                    16, 17, 18, 19, 20, 21, 22, 23,
                                    24, 25, 26, 27, 28, 29, 30, 31};

static constexpr uint32_t fanout256_heights[] = {
    1,  // size = 2^8
    2,  // size = 2^i, with i in [9,16]
    2, 2, 2, 2, 2, 2, 2,
    3,  // size = 2^i, with i in [17,24]
    3, 3, 3, 3, 3, 3, 3,
    4,  // size = 2^i, with i in [25,31]
    4, 4, 4, 4, 4, 4};

static constexpr uint32_t fanout64_heights[] = {
    2,  // size = 2^i, with i in [8,12]
    2, 2, 2, 2,
    3,  // size = 2^i, with i in [13,18]
    3, 3, 3, 3, 3,
    4,  // size = 2^i, with i in [19,24]
    4, 4, 4, 4, 4};

static constexpr uint32_t fanout16_heights[] = {
    2,           // size = 2^8
    3, 3, 3, 3,  // size = 2^i, with i in [9,12]
    4, 4, 4, 4   // size = 2^i, with i in [13,16]
};

#define GEN_TYPE_NODE256(i, Node) \
    segment_tree_simd<benchmarking::fanout256_heights[i], Node>
#define GEN_TYPE_NODE64(i, Node) \
    segment_tree_simd<benchmarking::fanout64_heights[i], Node>
#define GEN_TYPE_NODE16(i, Node) \
    segment_tree_simd<benchmarking::fanout16_heights[i], Node>

#define MAIN                                                         \
    if (argc < 2) {                                                  \
        std::cout << argv[0] << " type" << std::endl;                \
        return 1;                                                    \
    }                                                                \
    std::string type = argv[1];                                      \
    if (type == "st") {                                              \
        perf_test<segment_tree>();                                   \
    } else if (type == "sts_16u") {                                  \
        perf_segment_tree_simd_node16<node16u>();                    \
    } else if (type == "sts_64u") {                                  \
        perf_segment_tree_simd_node64<node64u>();                    \
    } else if (type == "sts_256u") {                                 \
        perf_segment_tree_simd_node256<node256u>();                  \
    } else if (type == "ft") {                                       \
        perf_test<fenwick_tree>();                                   \
    } else if (type == "ftb_16u") {                                  \
        perf_test<fenwick_tree_blocked<node16u>>();                  \
    } else if (type == "ftb_64u") {                                  \
        perf_test<fenwick_tree_blocked<node64u>>();                  \
    } else if (type == "ftb_256u") {                                 \
        perf_test<fenwick_tree_blocked<node256u>>();                 \
    } else if (type == "ftt_16u") {                                  \
        perf_test<fenwick_tree_truncated<node16u>>();                \
    } else if (type == "ftt_64u") {                                  \
        perf_test<fenwick_tree_truncated<node64u>>();                \
    } else if (type == "ftt_256u") {                                 \
        perf_test<fenwick_tree_truncated<node256u>>();               \
    } else {                                                         \
        std::cout << "unknown type \"" << type << "\"" << std::endl; \
        return 1;                                                    \
    }

#define PERF_SUM                                                \
    std::cout << tree.name() << "\n";                           \
    int64_t total = 0;                                          \
    essentials::timer_type t;                                   \
    static const int runs = 5;                                  \
    for (int run = 0; run != runs; ++run) {                     \
        t.start();                                              \
        for (auto q : queries) total += tree.sum(q);            \
        t.stop();                                               \
    }                                                           \
    std::cout << "# ignore: " << total << std::endl;            \
    double min = t.min();                                       \
    double avg_ns_query = (min * 1000) / queries.size();        \
    std::cout << "Mean per query: " << avg_ns_query << " [ns]"; \
    std::cout << std::endl;

#define PERF_UPDATE                                                   \
    std::cout << tree.name() << "\n";                                 \
    essentials::timer_type t;                                         \
    static const int runs = 5;                                        \
    for (int run = 0; run != runs; ++run) {                           \
        t.start();                                                    \
        for (auto const& q : queries) tree.update(q.first, q.second); \
        t.stop();                                                     \
    }                                                                 \
    std::cout << "# ignore: " << tree.sum(n - 1) << std::endl;        \
    double min = t.min();                                             \
    double avg_ns_query = (min * 1000) / queries.size();              \
    std::cout << "Mean per query: " << avg_ns_query << " [ns]";       \
    std::cout << std::endl;

}  // namespace benchmarking
}  // namespace psds
