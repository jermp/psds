#include "perf_common.hpp"

using namespace psds;

#define EXE_PERF_UPDATE(TREE, i)                                            \
    {                                                                       \
        TREE tree;                                                          \
        uint32_t n = uint32_t(1) << benchmarking::logs[i];                  \
        std::cout << "### n = " << n << std::endl;                          \
        {                                                                   \
            std::vector<int32_t> input(n);                                  \
            std::generate(input.begin(), input.end(),                       \
                          [&] { return distr_values.gen(); });              \
            tree.build(input.data(), n);                                    \
        }                                                                   \
        essentials::uniform_int_rng<int32_t> distr_queries(                 \
            0, n - 1, benchmarking::query_seed);                            \
        std::generate(queries.begin(), queries.end(), [&] {                 \
            return std::make_pair(distr_queries.gen(),                      \
                                  benchmarking::deltas[distr_posit.gen()]); \
        });                                                                 \
        PERF_UPDATE                                                         \
        std::cerr << avg_ns_query;                                          \
        if (benchmarking::logs[i] != benchmarking::max_log) {               \
            std::cerr << ", ";                                              \
            std::cout << std::endl;                                         \
        }                                                                   \
    }

template <typename Node>
void perf_test_tree_epi32_node256() {
    essentials::uniform_int_rng<int32_t> distr_values(-100, 100,
                                                      benchmarking::value_seed);
    essentials::uniform_int_rng<int32_t> distr_posit(0, 1,
                                                     benchmarking::posit_seed);
    std::vector<std::pair<uint32_t, int8_t>> queries(benchmarking::num_queries);

    std::cerr << "{";
    std::cerr << "\"type\": \"" << GEN_TYPE_NODE256(0, Node)::name() << "\", ";
    std::cerr << "\"timings\": [";

    EXE_PERF_UPDATE(GEN_TYPE_NODE256(0, Node), 0)
    EXE_PERF_UPDATE(GEN_TYPE_NODE256(1, Node), 1)
    EXE_PERF_UPDATE(GEN_TYPE_NODE256(2, Node), 2)
    EXE_PERF_UPDATE(GEN_TYPE_NODE256(3, Node), 3)
    EXE_PERF_UPDATE(GEN_TYPE_NODE256(4, Node), 4)
    EXE_PERF_UPDATE(GEN_TYPE_NODE256(5, Node), 5)
    EXE_PERF_UPDATE(GEN_TYPE_NODE256(6, Node), 6)
    EXE_PERF_UPDATE(GEN_TYPE_NODE256(7, Node), 7)
    EXE_PERF_UPDATE(GEN_TYPE_NODE256(8, Node), 8)
    EXE_PERF_UPDATE(GEN_TYPE_NODE256(9, Node), 9)
    EXE_PERF_UPDATE(GEN_TYPE_NODE256(10, Node), 10)
    EXE_PERF_UPDATE(GEN_TYPE_NODE256(11, Node), 11)
    EXE_PERF_UPDATE(GEN_TYPE_NODE256(12, Node), 12)
    EXE_PERF_UPDATE(GEN_TYPE_NODE256(13, Node), 13)
    EXE_PERF_UPDATE(GEN_TYPE_NODE256(14, Node), 14)
    EXE_PERF_UPDATE(GEN_TYPE_NODE256(15, Node), 15)
    EXE_PERF_UPDATE(GEN_TYPE_NODE256(16, Node), 16)

    std::cerr << "]}" << std::endl;
}

template <typename Node>
void perf_test_tree_epi32_node64() {
    essentials::uniform_int_rng<int32_t> distr_values(-100, 100,
                                                      benchmarking::value_seed);
    essentials::uniform_int_rng<int32_t> distr_posit(0, 1,
                                                     benchmarking::posit_seed);
    std::vector<std::pair<uint32_t, int8_t>> queries(benchmarking::num_queries);

    std::cerr << "{";
    std::cerr << "\"type\": \"" << GEN_TYPE_NODE64(0, Node)::name() << "\", ";
    std::cerr << "\"timings\": [";

    EXE_PERF_UPDATE(GEN_TYPE_NODE64(0, Node), 0)
    EXE_PERF_UPDATE(GEN_TYPE_NODE64(1, Node), 1)
    EXE_PERF_UPDATE(GEN_TYPE_NODE64(2, Node), 2)
    EXE_PERF_UPDATE(GEN_TYPE_NODE64(3, Node), 3)
    EXE_PERF_UPDATE(GEN_TYPE_NODE64(4, Node), 4)
    EXE_PERF_UPDATE(GEN_TYPE_NODE64(5, Node), 5)
    EXE_PERF_UPDATE(GEN_TYPE_NODE64(6, Node), 6)
    EXE_PERF_UPDATE(GEN_TYPE_NODE64(7, Node), 7)
    EXE_PERF_UPDATE(GEN_TYPE_NODE64(8, Node), 8)
    EXE_PERF_UPDATE(GEN_TYPE_NODE64(9, Node), 9)
    EXE_PERF_UPDATE(GEN_TYPE_NODE64(10, Node), 10)
    EXE_PERF_UPDATE(GEN_TYPE_NODE64(11, Node), 11)
    EXE_PERF_UPDATE(GEN_TYPE_NODE64(12, Node), 12)
    EXE_PERF_UPDATE(GEN_TYPE_NODE64(13, Node), 13)
    EXE_PERF_UPDATE(GEN_TYPE_NODE64(14, Node), 14)
    EXE_PERF_UPDATE(GEN_TYPE_NODE64(15, Node), 15)
    EXE_PERF_UPDATE(GEN_TYPE_NODE64(16, Node), 16)

    std::cerr << "]}" << std::endl;
}

template <typename Tree>
void perf_test_competitor() {
    essentials::uniform_int_rng<int32_t> distr_values(-100, 100,
                                                      benchmarking::value_seed);
    essentials::uniform_int_rng<int32_t> distr_posit(0, 1,
                                                     benchmarking::posit_seed);
    std::vector<std::pair<uint32_t, int8_t>> queries(benchmarking::num_queries);

    std::cerr << "{";
    std::cerr << "\"type\": \"" << Tree::name() << "\", ";
    std::cerr << "\"timings\": [";

    EXE_PERF_UPDATE(Tree, 0)
    EXE_PERF_UPDATE(Tree, 1)
    EXE_PERF_UPDATE(Tree, 2)
    EXE_PERF_UPDATE(Tree, 3)
    EXE_PERF_UPDATE(Tree, 4)
    EXE_PERF_UPDATE(Tree, 5)
    EXE_PERF_UPDATE(Tree, 6)
    EXE_PERF_UPDATE(Tree, 7)
    EXE_PERF_UPDATE(Tree, 8)
    EXE_PERF_UPDATE(Tree, 9)
    EXE_PERF_UPDATE(Tree, 10)
    EXE_PERF_UPDATE(Tree, 11)
    EXE_PERF_UPDATE(Tree, 12)
    EXE_PERF_UPDATE(Tree, 13)
    EXE_PERF_UPDATE(Tree, 14)
    EXE_PERF_UPDATE(Tree, 15)
    EXE_PERF_UPDATE(Tree, 16)

    std::cerr << "]}" << std::endl;
}

int main(int argc, char** argv) {
    MAIN return 0;
}
