#include <iostream>
#include <random>
#include <fstream>

#include "../external/essentials/include/essentials.hpp"
#include "types.hpp"

using namespace psds;

static constexpr int runs = 100;
static constexpr uint32_t num_queries = 10000;
static constexpr unsigned value_seed = 13;
static constexpr unsigned query_seed = 71;

static constexpr uint32_t logs[] = {8,  9,  10, 11, 12, 13, 14, 15,
                                    16, 17, 18, 19, 20, 21, 22, 23,
                                    24, 25, 26, 27, 28, 29, 30};

struct type_traits_256 {
    typedef node256u node_type;
    static constexpr uint32_t heights[] = {1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3,
                                           3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4

    };
};

struct type_traits_256_restricted {
    typedef node256u_restricted node_type;
    static constexpr uint32_t heights[] = {1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3,
                                           3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4

    };
};

struct type_traits_64 {
    typedef node64u node_type;
    static constexpr uint32_t heights[] = {2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4,
                                           4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5};
};

struct type_traits_64_restricted {
    typedef node64u_restricted node_type;
    static constexpr uint32_t heights[] = {2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4,
                                           4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5};
};

struct fake_type_traits {
    struct fake_node {};
    typedef fake_node node_type;
    static constexpr uint32_t heights[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
};

template <uint32_t Height, typename Node>
struct ft_wrapper {
    typedef fenwick_tree tree_type;
};

template <uint32_t Height, typename Node>
struct st_wrapper {
    typedef segment_tree tree_type;
};

template <uint32_t Height, typename Node>
struct ftt_64u_wrapper {
    typedef fenwick_tree_truncated<node64u> tree_type;
};

template <uint32_t Height, typename Node>
struct ftt_256u_wrapper {
    typedef fenwick_tree_truncated<node256u> tree_type;
};

template <int I, template <uint32_t H, typename Node> typename Tree,
          typename TypeTraits>
struct test {
    static void run(essentials::uniform_int_rng<int64_t>& distr_values,
                    std::vector<uint32_t>& queries, std::string& json,
                    std::string const& operation) {
        {
            typedef typename Tree<TypeTraits::heights[I],
                                  typename TypeTraits::node_type>::tree_type
                tree_type;
            tree_type tree;
            uint64_t n = uint64_t(1) << logs[I];
            std::cout << "### n = " << n << std::endl;
            {
                std::vector<int64_t> input(n);
                std::generate(input.begin(), input.end(),
                              [&] { return distr_values.gen(); });
                tree.build(input.data(), n);
            }
            essentials::uniform_int_rng<uint32_t> distr_queries(0, n - 1,
                                                                query_seed);
            std::generate(queries.begin(), queries.end(),
                          [&] { return distr_queries.gen(); });
            std::cout << tree.name() << "\n";

            essentials::timer_type t;
            double min = 0.0;
            double max = 0.0;
            double avg = 0.0;

            auto measure = [&]() {
                int64_t total = 0;
                if (operation == "sum") {
                    for (int run = 0; run != runs; ++run) {
                        t.start();
                        for (auto q : queries) total += tree.sum(q);
                        t.stop();
                    }
                } else if (operation == "update") {
                    for (int run = 0; run != runs; ++run) {
                        t.start();
                        for (auto const& q : queries) tree.update(q, q);
                        t.stop();
                    }
                    total = tree.sum(n - 1);
                } else {
                    assert(false);
                }
                std::cout << "# ignore: " << total << std::endl;
            };

            static constexpr int K = 10;

            // warm-up
            for (int k = 0; k != K; ++k) {
                measure();
                double avg_ns_query = (t.average() * 1000) / num_queries;
                avg += avg_ns_query;
                t.reset();
            }
            std::cout << "# warm-up: " << avg / K << std::endl;
            avg = 0.0;

            for (int k = 0; k != K; ++k) {
                measure();
                t.discard_max();
                double avg_ns_query = (t.max() * 1000) / num_queries;
                max += avg_ns_query;
                t.reset();
            }

            for (int k = 0; k != K; ++k) {
                measure();
                t.discard_min();
                t.discard_max();
                double avg_ns_query = (t.average() * 1000) / num_queries;
                avg += avg_ns_query;
                t.reset();
            }

            for (int k = 0; k != K; ++k) {
                measure();
                t.discard_min();
                double avg_ns_query = (t.min() * 1000) / num_queries;
                min += avg_ns_query;
                t.reset();
            }

            min /= K;
            max /= K;
            avg /= K;
            std::vector<double> tt{min, avg, max};
            std::sort(tt.begin(), tt.end());
            std::cout << "[" << tt[0] << "," << tt[1] << "," << tt[2] << "]\n";

            json += "[" + std::to_string(tt[0]) + "," + std::to_string(tt[1]) +
                    "," + std::to_string(tt[2]) + "],";
        }

        test<I + 1, Tree, TypeTraits>::run(distr_values, queries, json,
                                           operation);
    }
};

template <template <uint32_t H, typename Node> typename Tree,
          typename TypeTraits>
struct test<22 + 1, Tree, TypeTraits> {
    static inline void run(essentials::uniform_int_rng<int64_t>&,
                           std::vector<uint32_t>&, std::string&,
                           std::string const&) {}
};

template <template <uint32_t H, typename Node> typename Tree,
          typename TypeTraits>
void perf_test(std::string const& operation, std::string const& name) {
    essentials::uniform_int_rng<int64_t> distr_values(-100, 100, value_seed);
    std::vector<uint32_t> queries(num_queries);
    typedef
        typename Tree<0, typename TypeTraits::node_type>::tree_type tree_type;
    auto str = tree_type::name();
    if (name != "") str = name;
    std::string json("{\"type\":\"" + str + "\", \"timings\":[");

    test<0, Tree, TypeTraits>::run(distr_values, queries, json, operation);

    json.pop_back();
    json += "]}";
    std::cerr << json << std::endl;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " [type] [operation] --log [name]"
                  << std::endl;
        // todo: write list of possible type with full names
        // std::cout << "[type] is one among: ..." << std::endl;
        std::cout << "[operation] is either 'sum' or 'update'." << std::endl;
        return 1;
    }
    std::string type = argv[1];
    std::string operation = argv[2];
    std::string name("");
    if (argc > 3 and std::string(argv[3]) == "--log") {
        name = std::string(argv[4]);
    }

    if (type == "st") {
        perf_test<st_wrapper, fake_type_traits>(operation, name);
    } else if (type == "ft") {
        perf_test<ft_wrapper, fake_type_traits>(operation, name);
    } else if (type == "sts_64u") {
        perf_test<segment_tree_simd, type_traits_64>(operation, name);
    } else if (type == "sts_64u_restricted") {
        perf_test<segment_tree_simd, type_traits_64_restricted>(operation,
                                                                name);
    } else if (type == "sts_256u") {
        perf_test<segment_tree_simd, type_traits_256>(operation, name);
    } else if (type == "sts_256u_restricted") {
        perf_test<segment_tree_simd, type_traits_256_restricted>(operation,
                                                                 name);
        // } else if (type == "ftt_64u") {
        //     perf_test<ftt_64u_wrapper, fake_type_traits>(operation, name);
        // } else if (type == "ftt_256u") {
        //     perf_test<ftt_256u_wrapper, fake_type_traits>(operation, name);
    } else {
        std::cout << "unknown type \"" << type << "\"" << std::endl;
        return 1;
    }

    return 0;
}
