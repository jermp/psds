#include "test_common.hpp"
#include "test_tree.hpp"

// template <uint32_t Degree>
// struct node {
//     static constexpr uint32_t degree = Degree;
//     static constexpr uint32_t size = degree * 4;

//     static void build(int32_t const* input, uint8_t* out) {
//         std::fill(out, out + size, 0);
//         int32_t* S = reinterpret_cast<int32_t*>(out);
//         for (uint32_t i = 0; i != degree; ++i) S[i] = input[i];
//     }

//     static std::string name() {
//         return "node" + std::to_string(Degree);
//     }

//     node(uint8_t* ptr) {
//         S = reinterpret_cast<int32_t*>(ptr);
//     }

//     void update(uint32_t i, int8_t delta) {
//         assert(i < degree);
//         assert(delta == +1 or delta == -1);
//         S[i] += delta;
//     }

//     int32_t sum(uint32_t i) const {
//         assert(i < degree);
//         int32_t s = 0;
//         for (uint32_t k = 0; k <= i; ++k) s += S[k];
//         return s;
//     }

//     int32_t sum_back() const {
//         return sum(degree - 1);
//     }

// private:
//     int32_t* S;
// };

TEST_CASE("test blocked_fenwick_tree_epi32_node256u") {
    using namespace psds::testing;
    for (uint32_t log2 = min_log2; log2 != max_log2 + 1; ++log2) {
        test_tree<blocked_fenwick_tree_epi32<node256u>>(size_t(1) << log2);
    }
}
