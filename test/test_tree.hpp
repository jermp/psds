#pragma once

namespace psds {
namespace testing {

template <typename Tree>
void test_tree(size_t n) {
    essentials::uniform_int_rng<int32_t> distr(-100, 100,
                                               essentials::get_random_seed());
    std::cout << "building " << Tree::name() << " with " << n << " nodes"
              << std::endl;

    std::vector<int32_t> A(n);
    std::generate(A.begin(), A.end(), [&] { return distr.gen(); });
    Tree tree;
    tree.build(A.data(), n);

    for (uint32_t i = 0; i != n; ++i) {
        int32_t got = tree.sum(i);
        int32_t expected = 0;
        for (uint32_t k = 0; k != i + 1; ++k) expected += A[k];
        REQUIRE_MESSAGE(got == expected, "got sum(" << i << ") = " << got
                                                    << " but expected "
                                                    << expected);
    }

    auto update = [&](int8_t delta) {
        for (uint32_t run = 0; run != 500; ++run) {
            for (uint32_t i = 0; i != n; ++i) {
                tree.update(i, delta);
                int32_t got = tree.sum(i);
                A[i] += delta;
                int32_t expected = 0;
                for (uint32_t k = 0; k != i + 1; ++k) expected += A[k];
                REQUIRE_MESSAGE(got == expected,
                                "error during run "
                                    << run << ": got sum(" << i << ") = " << got
                                    << " but expected " << expected);
            }
        }
    };

    update(+1);
    update(-1);

    std::cout << "everything's good" << std::endl;
}

}  // namespace testing
}  // namespace psds