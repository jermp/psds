#pragma once

namespace psds {
namespace testing {

template <typename Tree>
void test_tree(size_t n) {
    essentials::uniform_int_rng<int32_t> distr(-1000000000, +1000000000,
                                               essentials::get_random_seed());
    std::cout << "== testing " << Tree::name() << " with " << n
              << " nodes ==" << std::endl;
    std::vector<int32_t> A(n);
    std::generate(A.begin(), A.end(), [&] { return distr.gen(); });
    Tree tree;
    essentials::logger("building tree...");
    tree.build(A.data(), n);

    {
        essentials::logger("testing sum queries...");
        int64_t expected = 0;
        for (uint32_t i = 0; i != n; ++i) {
            int64_t got = tree.sum(i);
            expected += A[i];
            REQUIRE_MESSAGE(got == expected, "got sum(" << i << ") = " << got
                                                        << " but expected "
                                                        << expected);
        }
    }

    auto update = [&](int8_t delta) {
        essentials::logger("testing update " + std::to_string(int(delta)) +
                           " queries...");
        static constexpr uint32_t queries = 5000;
        uint32_t step = n / queries;
        if (step == 0) step += 1;
        for (uint32_t run = 0; run != 256; ++run) {
            int64_t expected = 0;
            uint32_t k = 0;
            for (uint32_t i = 0; i < n; i += step) {
                tree.update(i, delta);
                int64_t got = tree.sum(i);
                A[i] += delta;
                for (; k != i + 1; ++k) expected += A[k];
                REQUIRE_MESSAGE(got == expected,
                                "error during run "
                                    << run << ": got sum(" << i << ") = " << got
                                    << " but expected " << expected);
            }
        }
    };

    update(+1);
    update(-1);

    std::cout << "\teverything's good" << std::endl;
}

}  // namespace testing
}  // namespace psds