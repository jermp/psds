#include <iostream>
#include <random>
#include <fstream>

#include "../external/essentials/include/essentials.hpp"
#include "../external/cmd_line_parser/include/parser.hpp"
#include "types.hpp"
#include "util.hpp"

#ifdef AVX_512
#include "nodes/node1024_restricted.hpp"
#endif

using namespace psds;

static constexpr int runs = 100;
static constexpr uint32_t num_queries = 10000;
static constexpr unsigned value_seed = 13;
static constexpr unsigned query_seed = 71;

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
struct ftt_64_wrapper {
    typedef fenwick_tree_truncated<node64> tree_type;
};

template <uint32_t, class>
struct ftt_64_restricted_wrapper {
    typedef fenwick_tree_truncated<node64_restricted> tree_type;
};

template <uint32_t, class>
struct ftt_256_wrapper {
    typedef fenwick_tree_truncated<node256> tree_type;
};

template <uint32_t, class>
struct ftt_256_restricted_wrapper {
    typedef fenwick_tree_truncated<node256_restricted> tree_type;
};

template <uint32_t, class>
struct ftb_64_wrapper {
    typedef fenwick_tree_blocked<node64> tree_type;
};

template <uint32_t, class>
struct ftb_64_restricted_wrapper {
    typedef fenwick_tree_blocked<node64_restricted> tree_type;
};

template <uint32_t, class>
struct ftb_256_wrapper {
    typedef fenwick_tree_blocked<node256> tree_type;
};

template <uint32_t, class>
struct ftb_256_restricted_wrapper {
    typedef fenwick_tree_blocked<node256_restricted> tree_type;
};

template <int I, template <uint32_t, class> typename Tree, typename Node>
struct test {
    static void run(essentials::uniform_int_rng<int64_t>& distr_values,
                    std::vector<uint32_t>& queries, std::string& json,
                    std::string const& operation, int i = -1) {
        if (i == -1 or i == I) {
            const uint64_t n = sizes[I];
            const uint32_t height = util::ceil_log(Node::fanout, n);
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
                return total;
            };

            static constexpr int K = 10;
            int64_t ignore = 0;

            // warm-up
            for (int k = 0; k != K; ++k) {
                ignore += measure();
                double avg_ns_query = (t.average() * 1000) / num_queries;
                avg += avg_ns_query;
                t.reset();
            }
            std::cout << "# warm-up: " << avg / K << std::endl;
            avg = 0.0;

            for (int k = 0; k != K; ++k) {
                ignore += measure();
                t.discard_max();
                double avg_ns_query = (t.max() * 1000) / num_queries;
                max += avg_ns_query;
                t.reset();
            }

            for (int k = 0; k != K; ++k) {
                ignore += measure();
                t.discard_min();
                t.discard_max();
                double avg_ns_query = (t.average() * 1000) / num_queries;
                avg += avg_ns_query;
                t.reset();
            }

            for (int k = 0; k != K; ++k) {
                ignore += measure();
                t.discard_min();
                double avg_ns_query = (t.min() * 1000) / num_queries;
                min += avg_ns_query;
                t.reset();
            }

            std::cout << "# ignore: " << ignore << std::endl;

            min /= K;
            max /= K;
            avg /= K;
            std::vector<double> tt{min, avg, max};
            std::sort(tt.begin(), tt.end());
            std::cout << "[" << tt[0] << "," << tt[1] << "," << tt[2] << "]\n";

            json += "[" + std::to_string(tt[0]) + "," + std::to_string(tt[1]) +
                    "," + std::to_string(tt[2]) + "],";
        }

        test<I + 1, Tree, Node>::run(distr_values, queries, json, operation, i);
    }
};

template <template <uint32_t, class> typename Tree, typename Node>
struct test<sizeof(sizes) / sizeof(sizes[0]), Tree, Node> {
    static inline void run(essentials::uniform_int_rng<int64_t>&,
                           std::vector<uint32_t>&, std::string&,
                           std::string const&, int) {}
};

template <template <uint32_t, class> typename Tree, typename Node>
void perf_test(std::string const& operation, std::string const& name,
               int i = -1) {
    essentials::uniform_int_rng<int64_t> distr_values(-100, 100, value_seed);
    std::vector<uint32_t> queries(num_queries);
    typedef typename Tree<1, Node>::tree_type tree_type;
    auto str = tree_type::name();
    if (name != "") str = name;
    std::string json("{\"type\":\"" + str + "\", ");
    if (i != -1) json += "\"n\":\"" + std::to_string(sizes[i]) + "\", ";
    json += "\"timings\":[";

    test<0, Tree, Node>::run(distr_values, queries, json, operation, i);

    json.pop_back();
    json += "]}";
    std::cerr << json << std::endl;
}

int main(int argc, char** argv) {
    cmd_line_parser::parser parser(argc, argv);
    parser.add("type", "Tree type.");
    parser.add("operation", "Either 'sum' or 'update'.");
    parser.add("name", "Friendly name.", "-n", false);
    parser.add("i",
               "Use a specific array size calculated as: floor(base^i) with "
               "base = 10^{1/10} = 1.25893. Running the program without this "
               "option will execute the benchmark for i = 24..90.",
               "-i", false);
    if (!parser.parse()) return 1;

    std::string name("");
    int i = -1;
    auto type = parser.get<std::string>("type");
    auto operation = parser.get<std::string>("operation");
    if (parser.parsed("name")) name = parser.get<std::string>("name");
    if (parser.parsed("i")) i = parser.get<int>("i");

    if (type == "st") {  // segment tree
        perf_test<st_wrapper, fake_node>(operation, name, i);

    } else if (type == "ft") {  // fenwick tree
        perf_test<ft_wrapper, fake_node>(operation, name, i);

    } else if (type == "sts_64") {  // segment tree with SIMD - fanout 64
        perf_test<segment_tree_simd, node64>(operation, name, i);
    } else if (type == "sts_64_restricted") {  // segment tree with SIMD -
                                               // fanout 64 - restricted case
        perf_test<segment_tree_simd, node64_restricted>(operation, name, i);
    } else if (type == "sts_256") {  // segment tree with SIMD - fanout 256
        perf_test<segment_tree_simd, node256>(operation, name, i);
    } else if (type == "sts_256_restricted") {  // segment tree with SIMD -
                                                // fanout 256 - restricted case
        perf_test<segment_tree_simd, node256_restricted>(operation, name, i);

#ifdef AVX_512
    } else if (type ==
               "sts_1024_restricted") {  // segment tree with SIMD AVX 512 -
                                         // fanout 1024 - restricted case
        perf_test<segment_tree_simd, node1024_restricted>(operation, name, i);
#endif

    } else if (type == "ftt_64") {  // fenwick tree truncated - fanout 64
        perf_test<ftt_64_wrapper, fake_node>(operation, name, i);
    } else if (type == "ftt_64_restricted") {  // fenwick tree truncated -
                                               // fanout 64 - restricted case
        perf_test<ftt_64_restricted_wrapper, fake_node>(operation, name, i);
    } else if (type == "ftt_256") {  // fenwick tree truncated - fanout 256
        perf_test<ftt_256_wrapper, fake_node>(operation, name, i);
    } else if (type == "ftt_256_restricted") {  // fenwick tree truncated -
                                                // fanout 256 - restricted case
        perf_test<ftt_256_restricted_wrapper, fake_node>(operation, name, i);

    } else if (type == "ftb_64") {  // fenwick tree blocked - fanout 64
        perf_test<ftb_64_wrapper, fake_node>(operation, name, i);
    } else if (type == "ftb_64_restricted") {  // fenwick tree blocked -
                                               // fanout 64 - restricted case
        perf_test<ftb_64_restricted_wrapper, fake_node>(operation, name, i);
    } else if (type == "ftb_256") {  // fenwick tree blocked - fanout 256
        perf_test<ftb_256_wrapper, fake_node>(operation, name, i);
    } else if (type == "ftb_256_restricted") {  // fenwick tree blocked -
                                                // fanout 256 - restricted case
        perf_test<ftb_256_restricted_wrapper, fake_node>(operation, name, i);

    } else {
        std::cout << "unknown type \"" << type << "\"" << std::endl;
        return 1;
    }

    return 0;
}
