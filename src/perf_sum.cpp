#include <iostream>
#include <random>

#include "perf_test_common.hpp"

using namespace psds;

#define EXE(TREE, i)                                                      \
    {                                                                     \
        TREE tree;                                                        \
                                                                          \
        uint32_t n = uint32_t(1) << benchmarking::logs[i];                \
        std::cout << "### n = " << n << std::endl;                        \
        {                                                                 \
            std::vector<int32_t> input(n);                                \
            std::generate(input.begin(), input.end(),                     \
                          [&] { return distr_values.gen(); });            \
            tree.build(input.data(), n);                                  \
        }                                                                 \
        essentials::uniform_int_rng<int32_t> distr_queries(0, n - 1, 71); \
        std::generate(queries.begin(), queries.end(),                     \
                      [&] { return distr_queries.gen(); });               \
        PERF_SUM                                                          \
                                                                          \
        std::cerr << avg_ns_query;                                        \
        if (benchmarking::logs[i] != benchmarking::max_log) {             \
            std::cerr << ", ";                                            \
            std::cout << std::endl;                                       \
        }                                                                 \
    }

template <typename Node>
void perf_test_tree_epi32_node256() {
    essentials::uniform_int_rng<int32_t> distr_values(-100, 100, 13);
    std::vector<uint32_t> queries(benchmarking::num_queries);

#define GEN_TYPE_NODE256(i) tree_epi32<benchmarking::degree256_heights[i], Node>

    std::cerr << "{";
    std::cerr << "\"type\": \"" << GEN_TYPE_NODE256(0)::name() << "\", ";
    std::cerr << "\"timings\": [";

    EXE(GEN_TYPE_NODE256(0), 0)
    EXE(GEN_TYPE_NODE256(1), 1)
    EXE(GEN_TYPE_NODE256(2), 2)
    EXE(GEN_TYPE_NODE256(3), 3)
    EXE(GEN_TYPE_NODE256(4), 4)
    EXE(GEN_TYPE_NODE256(5), 5)
    EXE(GEN_TYPE_NODE256(6), 6)
    EXE(GEN_TYPE_NODE256(7), 7)
    EXE(GEN_TYPE_NODE256(8), 8)
    EXE(GEN_TYPE_NODE256(9), 9)
    EXE(GEN_TYPE_NODE256(10), 10)
    EXE(GEN_TYPE_NODE256(11), 11)
    EXE(GEN_TYPE_NODE256(12), 12)
    EXE(GEN_TYPE_NODE256(13), 13)
    EXE(GEN_TYPE_NODE256(14), 14)
    EXE(GEN_TYPE_NODE256(15), 15)
    EXE(GEN_TYPE_NODE256(16), 16)

    std::cerr << "]}" << std::endl;
}

template <typename Node>
void perf_test_tree_epi32_node64() {
    essentials::uniform_int_rng<int32_t> distr_values(-100, 100, 13);
    std::vector<uint32_t> queries(benchmarking::num_queries);

#define GEN_TYPE_NODE64(i) tree_epi32<benchmarking::degree64_heights[i], Node>

    std::cerr << "{";
    std::cerr << "\"type\": \"" << GEN_TYPE_NODE64(0)::name() << "\", ";
    std::cerr << "\"timings\": [";

    EXE(GEN_TYPE_NODE64(0), 0)
    EXE(GEN_TYPE_NODE64(1), 1)
    EXE(GEN_TYPE_NODE64(2), 2)
    EXE(GEN_TYPE_NODE64(3), 3)
    EXE(GEN_TYPE_NODE64(4), 4)
    EXE(GEN_TYPE_NODE64(5), 5)
    EXE(GEN_TYPE_NODE64(6), 6)
    EXE(GEN_TYPE_NODE64(7), 7)
    EXE(GEN_TYPE_NODE64(8), 8)
    EXE(GEN_TYPE_NODE64(9), 9)
    EXE(GEN_TYPE_NODE64(10), 10)
    EXE(GEN_TYPE_NODE64(11), 11)
    EXE(GEN_TYPE_NODE64(12), 12)
    EXE(GEN_TYPE_NODE64(13), 13)
    EXE(GEN_TYPE_NODE64(14), 14)
    EXE(GEN_TYPE_NODE64(15), 15)
    EXE(GEN_TYPE_NODE64(16), 16)

    std::cerr << "]}" << std::endl;
}

template <typename Tree>
void perf_test_competitor() {
    essentials::uniform_int_rng<int32_t> distr_values(-100, 100, 13);
    std::vector<uint32_t> queries(benchmarking::num_queries);

    std::cerr << "{";
    std::cerr << "\"type\": \"" << Tree::name() << "\", ";
    std::cerr << "\"timings\": [";

    EXE(Tree, 0)
    EXE(Tree, 1)
    EXE(Tree, 2)
    EXE(Tree, 3)
    EXE(Tree, 4)
    EXE(Tree, 5)
    EXE(Tree, 6)
    EXE(Tree, 7)
    EXE(Tree, 8)
    EXE(Tree, 9)
    EXE(Tree, 10)
    EXE(Tree, 11)
    EXE(Tree, 12)
    EXE(Tree, 13)
    EXE(Tree, 14)
    EXE(Tree, 15)
    EXE(Tree, 16)

    std::cerr << "]}" << std::endl;
}

int main(int argc, char** argv) {
    MAIN return 0;
}
