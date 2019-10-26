#include <iostream>
#include <random>

#include "perf_test_common.hpp"

using namespace dps;

template <typename Tree>
void perf_test() {
    std::mt19937_64 rng_values(13);
    std::mt19937_64 rng_queries(71);
    std::mt19937_64 rng_pos(99);

    std::uniform_int_distribution<int32_t> distr_values(-100, 100);
    std::uniform_int_distribution<int> distr_pos(0, 1);
    static int8_t deltas[] = {+1, -1};

    static constexpr uint32_t num_queries = 1000000;
    static constexpr uint32_t max_log = 24;
    std::vector<std::pair<uint32_t, int8_t>> queries(num_queries);

    std::cerr << "{";
    std::cerr << "\"type\": \"" << Tree::name() << "\", ";
    std::cerr << "\"timings\": [";

    for (uint32_t log2 = 8; log2 != max_log + 1; ++log2) {
        uint32_t n = uint32_t(1) << log2;
        std::cout << "### n = " << n << std::endl;

        Tree tree;
        {
            std::vector<int32_t> input;
            input.reserve(n);
            for (uint32_t i = 0; i != n; ++i) {
                input.push_back(distr_values(rng_values));
            }
            tree.build(input.data(), n);
        }

        std::uniform_int_distribution<uint32_t> distr_queries(0, n - 1);

        for (uint32_t i = 0; i != num_queries; ++i) {
            queries[i] = {distr_queries(rng_queries),
                          deltas[distr_pos(rng_pos)]};
        }

        PERF_UPDATE

        std::cerr << avg_ns_query;
        if (log2 != max_log) {
            std::cerr << ", ";
            std::cout << std::endl;
        }
    }

    std::cerr << "]}" << std::endl;
}

int main(int argc, char** argv) {
    MAIN return 0;
}