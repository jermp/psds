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

constexpr size_t ceil_log2(size_t n) {
    return (n < 2) ? 1 : 1 + ceil_log2(n / 2);
}

constexpr size_t floor_log2(size_t n) {
    return (n == 1) ? 0 : 1 + floor_log2(n / 2);
}

constexpr double const_ceil(double val) {
    const auto val_int = (int64_t)val;
    const double fval_int = (double)val_int;
    return (val < double(0) ? fval_int
                            : (val == fval_int ? val : fval_int + double(1)));
}

constexpr uint32_t ceil_log(uint32_t base, uint32_t n) {
    assert(base > 0);
    assert((base & (base - 1)) == 0);  // base must be a power of 2
    return const_ceil(static_cast<double>(ceil_log2(n)) /
                      static_cast<double>(floor_log2(base)));
}

static constexpr uint32_t sizes[] = {
    251,       316,       398,       501,       630,       794,       1000,
    1258,      1584,      1995,      2511,      3162,      3981,      5011,
    6309,      7943,      10000,     12589,     15848,     19952,     25118,
    31622,     39810,     50118,     63095,     79432,     100000,    125892,
    158489,    199526,    251188,    316227,    398107,    501187,    630957,
    794328,    1000000,   1258925,   1584893,   1995262,   2511886,   3162277,
    3981071,   5011872,   6309573,   7943282,   10000000,  12589254,  15848931,
    19952623,  25118864,  31622776,  39810717,  50118723,  63095734,  79432823,
    100000000, 125892541, 158489319, 199526231, 251188643, 316227766, 398107170,
    501187233, 630957344, 794328234, 1000000000};

struct fake_node {
    static constexpr uint64_t fanout = 2;
};

template <uint32_t, class>
struct ft_wrapper {
    typedef fenwick_tree tree_type;
};

template <uint32_t, class>
struct st_wrapper {
    typedef segment_tree tree_type;
};

template <uint32_t, class>
struct ftt_64u_wrapper {
    typedef fenwick_tree_truncated<node64u> tree_type;
};

template <uint32_t, class>
struct ftt_64u_restricted_wrapper {
    typedef fenwick_tree_truncated<node64u_restricted> tree_type;
};

template <uint32_t, class>
struct ftt_256u_wrapper {
    typedef fenwick_tree_truncated<node256u> tree_type;
};

template <uint32_t, class>
struct ftt_256u_restricted_wrapper {
    typedef fenwick_tree_truncated<node256u_restricted> tree_type;
};

template <uint32_t, class>
struct ftb_64u_wrapper {
    typedef fenwick_tree_blocked<node64u> tree_type;
};

template <uint32_t, class>
struct ftb_64u_restricted_wrapper {
    typedef fenwick_tree_blocked<node64u_restricted> tree_type;
};

template <uint32_t, class>
struct ftb_256u_wrapper {
    typedef fenwick_tree_blocked<node256u> tree_type;
};

template <uint32_t, class>
struct ftb_256u_restricted_wrapper {
    typedef fenwick_tree_blocked<node256u_restricted> tree_type;
};

template <int I, template <uint32_t, class> typename Tree, typename Node>
struct test {
    static void run(essentials::uniform_int_rng<int64_t>& distr_values,
                    std::vector<uint32_t>& queries, std::string& json,
                    std::string const& operation) {
        {
            const uint64_t n = sizes[I];
            const uint32_t height = ceil_log(Node::fanout, n);
            typedef typename Tree<height, Node>::tree_type tree_type;
            tree_type tree;

            std::cout << "### n = " << n << "; height = " << height
                      << std::endl;

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

        test<I + 1, Tree, Node>::run(distr_values, queries, json, operation);
    }
};

template <template <uint32_t, class> typename Tree, typename Node>
struct test<sizeof(sizes) / sizeof(sizes[0]), Tree, Node> {
    static inline void run(essentials::uniform_int_rng<int64_t>&,
                           std::vector<uint32_t>&, std::string&,
                           std::string const&) {}
};

template <template <uint32_t, class> typename Tree, typename Node>
void perf_test(std::string const& operation, std::string const& name) {
    essentials::uniform_int_rng<int64_t> distr_values(-100, 100, value_seed);
    std::vector<uint32_t> queries(num_queries);
    typedef typename Tree<0, Node>::tree_type tree_type;
    auto str = tree_type::name();
    if (name != "") str = name;
    std::string json("{\"type\":\"" + str + "\", \"timings\":[");

    test<0, Tree, Node>::run(distr_values, queries, json, operation);

    json.pop_back();
    json += "]}";
    std::cerr << json << std::endl;
}

int main(int argc, char** argv) {
    if (argc < 3) {
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

    if (type == "st") {  // segment tree
        perf_test<st_wrapper, fake_node>(operation, name);

    } else if (type == "ft") {  // fenwick tree
        perf_test<ft_wrapper, fake_node>(operation, name);

    } else if (type == "sts_64u") {  // segment tree with SIMD - fanout 64
        perf_test<segment_tree_simd, node64u>(operation, name);
    } else if (type == "sts_64u_restricted") {  // segment tree with SIMD -
                                                // fanout 64 - restricted case
        perf_test<segment_tree_simd, node64u_restricted>(operation, name);
    } else if (type == "sts_256u") {  // segment tree with SIMD - fanout 256
        perf_test<segment_tree_simd, node256u>(operation, name);
    } else if (type == "sts_256u_restricted") {  // segment tree with SIMD -
                                                 // fanout 256 - restricted case
        perf_test<segment_tree_simd, node256u_restricted>(operation, name);

    } else if (type == "ftt_64u") {  // fenwick tree truncated - fanout 64
        perf_test<ftt_64u_wrapper, fake_node>(operation, name);
    } else if (type == "ftt_64u_restricted") {  // fenwick tree truncated -
                                                // fanout 64 - restricted case
        perf_test<ftt_64u_restricted_wrapper, fake_node>(operation, name);
    } else if (type == "ftt_256u") {  // fenwick tree truncated - fanout 256
        perf_test<ftt_256u_wrapper, fake_node>(operation, name);
    } else if (type == "ftt_256u_restricted") {  // fenwick tree truncated -
                                                 // fanout 256 - restricted case
        perf_test<ftt_256u_restricted_wrapper, fake_node>(operation, name);

    } else if (type == "ftb_64u") {  // fenwick tree blocked - fanout 64
        perf_test<ftb_64u_wrapper, fake_node>(operation, name);
    } else if (type == "ftb_64u_restricted") {  // fenwick tree blocked -
                                                // fanout 64 - restricted case
        perf_test<ftb_64u_restricted_wrapper, fake_node>(operation, name);
    } else if (type == "ftb_256u") {  // fenwick tree blocked - fanout 256
        perf_test<ftb_256u_wrapper, fake_node>(operation, name);
    } else if (type == "ftb_256u_restricted") {  // fenwick tree blocked -
                                                 // fanout 256 - restricted case
        perf_test<ftb_256u_restricted_wrapper, fake_node>(operation, name);

    } else {
        std::cout << "unknown type \"" << type << "\"" << std::endl;
        return 1;
    }

    return 0;
}
