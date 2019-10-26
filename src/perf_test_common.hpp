#pragma once

#include <fstream>
#include "../external/essentials/include/essentials.hpp"
#include "types.hpp"

#define MAIN                                          \
    if (argc < 2) {                                   \
        std::cout << argv[0] << " type" << std::endl; \
        return 1;                                     \
    }                                                 \
                                                      \
    std::string type = argv[1];                       \
                                                      \
    if (type == "tree256u") {                         \
        perf_test<tree256u>();                        \
    } else if (type == "tree256s") {                  \
        perf_test<tree256s>();                        \
    } else if (type == "tree64u") {                   \
        perf_test<tree64u>();                         \
    } else if (type == "tree64s") {                   \
        perf_test<tree64s>();                         \
    } else if (type == "stv") {                       \
        perf_test<segment_tree_vanilla<int32_t>>();   \
    } else if (type == "ft") {                        \
        perf_test<fenwick_tree<int32_t>>();           \
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
        for (auto q : queries) {                                   \
            total += tree.sum(q);                                  \
        }                                                          \
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
        for (auto const& q : queries) {                               \
            tree.update(q.first, q.second);                           \
        }                                                             \
        t.stop();                                                     \
    }                                                                 \
    std::cout << tree.name() << " -- ";                               \
    std::cout << tree.sum(n - 1) << std::endl;                        \
    double min = t.min();                                             \
    double avg_ns_query = (min * 1000) / queries.size();              \
    std::cout << "Mean per run: " << min << " [musec]\n";             \
    std::cout << "Mean per query: " << avg_ns_query << " [ns]";       \
    std::cout << std::endl;
