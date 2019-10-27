#include "test_common.hpp"

template <typename Node>
void test() {
    essentials::uniform_int_rng<int32_t> distr(-100, 100,
                                               essentials::get_random_seed());
    std::vector<int32_t> A(Node::degree);
    std::vector<uint8_t> out(Node::size);
    std::generate(A.begin(), A.end(), [&] { return distr.gen(); });
    Node::build(A.data(), out.data());
    Node n(out.data());

    std::cout << "sum queries" << std::endl;
    for (uint32_t i = 0; i != Node::degree; ++i) {
        int32_t got = n.sum(i);
        int32_t expected = 0;
        for (uint32_t k = 0; k != i + 1; ++k) expected += A[k];
        REQUIRE_MESSAGE(got == expected, "got sum(" << i << ") = " << got
                                                    << " but expected "
                                                    << expected);
    }

    auto update = [&](int8_t delta) {
        std::cout << "update " << int(delta) << " queries" << std::endl;
        for (uint32_t run = 0; run != 500; ++run) {
            for (uint32_t i = 0; i != Node::degree; ++i) {
                n.update(i, delta);
                int32_t got = n.sum(i);
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

TEST_CASE("test node64u") {
    test<node64u>();
}

TEST_CASE("test node64s") {
    test<node64s>();
}

TEST_CASE("test node256u") {
    test<node256u>();
}

TEST_CASE("test node256s") {
    test<node256s>();
}
