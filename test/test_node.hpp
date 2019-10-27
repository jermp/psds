#pragma once

namespace psds {
namespace testing {

template <typename Node>
void test_node() {
    essentials::uniform_int_rng<int32_t> distr(-100, 100,
                                               essentials::get_random_seed());
    std::cout << "testing " << Node::name() << " with degree " << Node::degree
              << std::endl;
    std::vector<int32_t> A(Node::degree);
    std::vector<uint8_t> out(Node::size);
    std::generate(A.begin(), A.end(), [&] { return distr.gen(); });
    Node::build(A.data(), out.data());
    Node node(out.data());

    {
        int32_t expected = 0;
        for (uint32_t i = 0; i != Node::degree; ++i) {
            int32_t got = node.sum(i);
            expected += A[i];
            REQUIRE_MESSAGE(got == expected, "got sum(" << i << ") = " << got
                                                        << " but expected "
                                                        << expected);
        }
    }

    auto update = [&](int8_t delta) {
        for (uint32_t run = 0; run != 256; ++run) {
            int32_t expected = 0;
            for (uint32_t i = 0; i != Node::degree; ++i) {
                node.update(i, delta);
                int32_t got = node.sum(i);
                A[i] += delta;
                expected += A[i];
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