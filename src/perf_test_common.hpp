#pragma once

#include <fstream>
#include "../external/essentials/include/essentials.hpp"
#include "types.hpp"

namespace psds {
namespace benchmarking {

static constexpr uint32_t num_queries = 1000000;
static constexpr uint32_t max_log = 24;

static constexpr uint32_t logs[] = {8,  9,  10, 11, 12, 13, 14, 15, 16,
                                    17, 18, 19, 20, 21, 22, 23, 24};

static constexpr uint32_t degree256_heights[] = {
    1,  // size = 2^8
    2,  // size = 2^i, with i in [9,16]
    2, 2, 2, 2, 2, 2, 2,
    3,  // size = 2^i, with i in [17,24]
    3, 3, 3, 3, 3, 3, 3};

static constexpr uint32_t degree64_heights[] = {
    2,  // size = 2^i, with i in [8,12]
    2, 2, 2, 2,
    3,  // size = 2^i, with i in [13,18]
    3, 3, 3, 3, 3,
    4,  // size = 2^i, with i in [19,24]
    4, 4, 4, 4, 4};

#define MAIN                                          \
    if (argc < 2) {                                   \
        std::cout << argv[0] << " type" << std::endl; \
        return 1;                                     \
    }                                                 \
                                                      \
    std::string type = argv[1];                       \
                                                      \
    if (type == "tree256u") {                         \
        perf_test_tree_epi32_node256<node256u>();     \
    } else if (type == "tree256s") {                  \
        perf_test_tree_epi32_node256<node256s>();     \
    } else if (type == "tree64u") {                   \
        perf_test_tree_epi32_node64<node64u>();       \
    } else if (type == "tree64s") {                   \
        perf_test_tree_epi32_node64<node64s>();       \
    } else if (type == "stv") {                       \
        perf_test_competitor<segment_tree_type>();    \
    } else if (type == "ft") {                        \
        perf_test_competitor<fenwick_tree_type>();    \
    } else {                                          \
        return 1;                                     \
    }

#define PERF_SUM                                                   \
    std::cout << "performing " << queries.size() << " SUM queries" \
              << std::endl;                                        \
    int64_t total = 0;                                             \
    essentials::timer_type t;                                      \
    static const int runs = 10 + 1;                                \
    for (int run = 0; run != runs; ++run) {                        \
        t.start();                                                 \
        for (auto q : queries) total += tree.sum(q);               \
        t.stop();                                                  \
    }                                                              \
    std::cout << tree.name() << " -- ";                            \
    std::cout << total << std::endl;                               \
    double min = t.min();                                          \
    double avg_ns_query = (min * 1000) / queries.size();           \
    std::cout << "Mean per run: " << min << " [musec]\n";          \
    std::cout << "Mean per query: " << avg_ns_query << " [ns]";    \
    std::cout << std::endl;

#define PERF_UPDATE                                                   \
    std::cout << "performing " << queries.size() << " UPDATE queries" \
              << std::endl;                                           \
    essentials::timer_type t;                                         \
    static const int runs = 5 + 1;                                    \
    for (int run = 0; run != runs; ++run) {                           \
        t.start();                                                    \
        for (auto const& q : queries) tree.update(q.first, q.second); \
        t.stop();                                                     \
    }                                                                 \
    std::cout << tree.name() << " -- ";                               \
    std::cout << tree.sum(n - 1) << std::endl;                        \
    double min = t.min();                                             \
    double avg_ns_query = (min * 1000) / queries.size();              \
    std::cout << "Mean per run: " << min << " [musec]\n";             \
    std::cout << "Mean per query: " << avg_ns_query << " [ns]";       \
    std::cout << std::endl;

}  // namespace benchmarking
}  // namespace psds
