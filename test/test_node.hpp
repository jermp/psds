#pragma once

namespace psds {
namespace testing {

template <typename Node>
void test_node() {
    essentials::uniform_int_rng<int64_t> distr(-100, +1000000000,
                                               essentials::get_random_seed());
    std::cout << "testing " << Node::name() << " with fanout " << Node::fanout
              << std::endl;
    std::vector<int64_t> A(Node::fanout);
    std::vector<uint8_t> out(Node::bytes);
    std::generate(A.begin(), A.end(), [&] { return distr.gen(); });
    Node::build(A.data(), out.data());
    Node node(out.data());

    {
        int64_t expected = 0;
        for (uint32_t i = 0; i != Node::fanout; ++i) {
            int64_t got = node.sum(i);
            expected += A[i];
            REQUIRE_MESSAGE(got == expected, "got sum(" << i << ") = " << got
                                                        << " but expected "
                                                        << expected);
        }
    }

    auto update = [&](int64_t delta) {
        for (uint32_t run = 0; run != 100; ++run) {
            int64_t expected = 0;
            for (uint32_t i = 0; i != Node::fanout; ++i) {
                node.update(i, delta);
                int64_t got = node.sum(i);
                A[i] += delta;
                expected += A[i];
                REQUIRE_MESSAGE(got == expected,
                                "error during run "
                                    << run << ": got sum(" << i << ") = " << got
                                    << " but expected " << expected);
            }
        }
    };

    int64_t delta = distr.gen();
    update(delta);

    std::cout << "everything's good" << std::endl;
}

}  // namespace testing
}  // namespace psds