#include <iostream>
#include <vector>

#include "node256u.hpp"
#include "node256s.hpp"

using namespace dps;

template <typename Node>
void test(std::vector<int32_t>& A) {
    typename Node::builder builder;
    std::vector<uint8_t> out(Node::size);
    builder.build(A.data(), out.data());
    Node n(out.data());

    std::cout << "sum queries" << std::endl;
    for (uint32_t i = 0; i != 256; ++i) {
        int32_t s = n.sum(i);
        int32_t expected = 0;
        for (uint32_t k = 0; k != i + 1; ++k) {
            expected += A[k];
        }
        if (s != expected) {
            std::cout << "got sum(" << i << ") = " << s << " but expected "
                      << expected << std::endl;
        }
    }

    std::cout << "update +1 queries" << std::endl;
    for (uint32_t run = 0; run != 500; ++run) {
        for (uint32_t i = 0; i != 256; ++i) {
            n.update(i, 1);
            int32_t s = n.sum(i);
            A[i] += 1;
            int32_t expected = 0;
            for (uint32_t k = 0; k != i + 1; ++k) {
                expected += A[k];
            }
            if (s != expected) {
                std::cout << "error at run " << run << std::endl;
                std::cout << "got sum(" << i << ") = " << s << " but expected "
                          << expected << std::endl;
            }
        }
    }

    std::cout << "update -1 queries" << std::endl;
    for (uint32_t run = 0; run != 500; ++run) {
        for (uint32_t i = 0; i != 256; ++i) {
            n.update(i, -1);
            int32_t s = n.sum(i);
            A[i] -= 1;
            int32_t expected = 0;
            for (uint32_t k = 0; k != i + 1; ++k) {
                expected += A[k];
            }
            if (s != expected) {
                std::cout << "error at run " << run << std::endl;
                std::cout << "got sum(" << i << ") = " << s << " but expected "
                          << expected << std::endl;
            }
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << argv[0] << " type < input_array" << std::endl;
        return 1;
    }

    std::string type(argv[1]);

    std::vector<int32_t> A;
    A.reserve(256);

    for (int i = 0; i != 256; ++i) {
        int32_t x;
        std::cin >> x;
        A.push_back(x);
    }

    if (type == "node256u") {
        test<node256u>(A);
    } else if (type == "node256s") {
        test<node256s>(A);
    } else {
        std::cerr << "unknown type '" << type << "'" << std::endl;
        return 1;
    }

    return 0;
}