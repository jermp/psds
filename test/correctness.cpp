#include <iostream>
#include <vector>

#include "../external/essentials/include/essentials.hpp"
#include "types.hpp"

using namespace psds;

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
        int32_t s = n.sum(i);
        int32_t expected = 0;
        for (uint32_t k = 0; k != i + 1; ++k) expected += A[k];
        if (s != expected) {
            std::cout << "got sum(" << i << ") = " << s << " but expected "
                      << expected << std::endl;
            return;
        }
    }

    std::cout << "update +1 queries" << std::endl;
    for (uint32_t run = 0; run != 500; ++run) {
        for (uint32_t i = 0; i != Node::degree; ++i) {
            n.update(i, 1);
            int32_t s = n.sum(i);
            A[i] += 1;
            int32_t expected = 0;
            for (uint32_t k = 0; k != i + 1; ++k) expected += A[k];
            if (s != expected) {
                std::cout << "error at run " << run << std::endl;
                std::cout << "got sum(" << i << ") = " << s << " but expected "
                          << expected << std::endl;
                return;
            }
        }
    }

    std::cout << "update -1 queries" << std::endl;
    for (uint32_t run = 0; run != 500; ++run) {
        for (uint32_t i = 0; i != Node::degree; ++i) {
            n.update(i, -1);
            int32_t s = n.sum(i);
            A[i] -= 1;
            int32_t expected = 0;
            for (uint32_t k = 0; k != i + 1; ++k) expected += A[k];
            if (s != expected) {
                std::cout << "error at run " << run << std::endl;
                std::cout << "got sum(" << i << ") = " << s << " but expected "
                          << expected << std::endl;
                return;
            }
        }
    }

    std::cout << "everything's good" << std::endl;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << argv[0] << " type" << std::endl;
        return 1;
    }

    std::string type(argv[1]);

    if (type == "node256u") {
        test<node256u>();
    } else if (type == "node256s") {
        test<node256s>();
    } else if (type == "node64u") {
        test<node64u>();
    } else if (type == "node64s") {
        test<node64s>();
    } else {
        std::cerr << "unknown type '" << type << "'" << std::endl;
        return 1;
    }

    return 0;
}