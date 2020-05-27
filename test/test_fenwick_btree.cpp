#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include <random>

unsigned get_random_seed() {
    return std::chrono::system_clock::now().time_since_epoch().count();
}

template <typename IntType>
struct uniform_int_rng {
    uniform_int_rng(IntType from, IntType to, unsigned seed = get_random_seed())
        : m_rng(seed), m_distr(from, to) {}

    IntType gen() {
        return m_distr(m_rng);
    }

private:
    std::mt19937_64 m_rng;
    std::uniform_int_distribution<IntType> m_distr;
};

template <typename Tree>
void test_tree(uint64_t n) {
    uniform_int_rng<int64_t> distr(-100, +100);
    std::cout << "== testing " << Tree::name() << " with " << n
              << " nodes ==" << std::endl;
    std::vector<int64_t> A(n);
    std::generate(A.begin(), A.end(), [&] { return distr.gen(); });
    Tree tree;
    std::cout << "building tree..." << std::endl;
    tree.build(A.data(), n);

    {
        std::cout << "testing sum queries..." << std::endl;
        static constexpr uint32_t sum_queries = 5000;
        uint32_t step = n / sum_queries;
        if (step == 0) step = 1;
        int64_t expected = 0;
        uint32_t k = 0;
        for (uint32_t i = 0; i < n; i += step) {
            int64_t got = tree.sum(i);
            for (; k != i + 1; ++k) expected += A[k];
            if (got != expected) {
                std::cout << "got sum(" << i << ") = " << got
                          << " but expected " << expected << std::endl;
            }
        }
    }

    auto update = [&](int8_t delta) {
        std::cout << "testing update queries..." << std::endl;
        static constexpr uint32_t update_queries = 5000;
        uint32_t step = n / update_queries;
        if (step == 0) step = 1;
        for (uint32_t run = 0; run != 100; ++run) {
            int64_t expected = 0;
            uint32_t k = 0;
            for (uint32_t i = 0; i < n; i += step) {
                tree.update(i, delta);
                int64_t got = tree.sum(i);
                A[i] += delta;
                for (; k != i + 1; ++k) expected += A[k];
                if (got != expected) {
                    std::cout << "error during run " << run << ": got sum(" << i
                              << ") = " << got << " but expected " << expected
                              << std::endl;
                }
            }
        }
    };

    int8_t delta = distr.gen();
    std::cout << "delta " << int64_t(delta) << std::endl;
    update(delta);
    std::cout << "\teverything's good" << std::endl;
}

struct node4 {
    static const uint64_t fanout = 4;
    static const uint64_t log2_fanout = 2;
    static const uint64_t bytes = fanout * sizeof(int64_t);

    template <typename T>
    static void build(T const* input, uint8_t* out) {
        std::fill(out, out + fanout * sizeof(T), 0);
        int64_t* keys = reinterpret_cast<int64_t*>(out);
        keys[0] = input[0];
        for (uint64_t i = 1; i != fanout; ++i) keys[i] = keys[i - 1] + input[i];
    }

    static std::string name() {
        return "node4";
    }

    node4(uint8_t* ptr) {
        at(ptr);
    }

    inline void at(uint8_t* ptr) {
        keys = reinterpret_cast<int64_t*>(ptr);
    }

    int64_t sum(uint64_t i) const {
        assert(i < fanout);
        return keys[i];
    }

    void update(uint64_t i, int64_t delta) {
        for (uint64_t j = i; j < fanout; ++j) keys[j] += delta;
    }

    int64_t back() const {
        return keys[fanout - 1] - keys[fanout - 2];
    }

    void update_back(int64_t delta) {
        keys[fanout - 1] += delta;
    }

    int64_t* keys;
};

template <typename T>
void build_node_prefix_sums(T const* input, uint8_t* out, uint64_t segment_size,
                            uint64_t bytes) {
    std::fill(out, out + bytes, 0);
    int64_t* summary = reinterpret_cast<int64_t*>(out);
    int64_t* keys = summary + segment_size;
    summary[0] = 0;
    for (uint64_t i = 0; i != segment_size; ++i) {
        keys[0] = input[0];
        for (uint64_t j = 1; j != segment_size; ++j) {
            keys[j] = keys[j - 1] + input[j];
        }
        if (i + 1 < segment_size) {
            summary[i + 1] = summary[i] + keys[segment_size - 1];
        }
        input += segment_size;
        keys += segment_size;
    }
}

struct node64 {
    static constexpr uint64_t fanout = 64;
    static const uint64_t log2_fanout = 6;
    static constexpr uint64_t segment_size = 8;
    static constexpr uint64_t bytes = (fanout + segment_size) * sizeof(int64_t);

    template <typename T>
    static void build(T const* input, uint8_t* out) {
        build_node_prefix_sums(input, out, segment_size, bytes);
    }

    static std::string name() {
        return "node64";
    }

    node64(uint8_t* ptr) {
        at(ptr);
    }

    inline void at(uint8_t* ptr) {
        summary = reinterpret_cast<int64_t*>(ptr);
        keys = reinterpret_cast<int64_t*>(ptr + segment_size * sizeof(int64_t));
    }

    void update(uint64_t i, int64_t delta) {
        if (i == fanout) return;
        assert(i < fanout);
        uint64_t j = i / segment_size;
        uint64_t k = i % segment_size;
        for (uint64_t z = j + 1; z != segment_size; ++z) summary[z] += delta;
        for (uint64_t z = k, base = j * segment_size; z != segment_size; ++z) {
            keys[base + z] += delta;
        }
    }

    int64_t sum(uint64_t i) const {
        assert(i < fanout);
        return summary[i / segment_size] + keys[i];
    }

    int64_t back() const {
        return keys[fanout - 1] - keys[fanout - 2];
    }

    void update_back(int64_t delta) {
        keys[fanout - 1] += delta;
    }

private:
    int64_t* summary;
    int64_t* keys;
};

template <typename Node>
struct fenwick_btree {
    fenwick_btree() : m_blocks(0), m_size(0), m_ptr(nullptr) {}

    static const uint64_t b = Node::fanout;
    static const uint64_t log2_b = Node::log2_fanout;
    static_assert((b & (b - 1)) == 0, "Node::fanout must be a per of 2");

    template <typename T>
    void build(T const* input, uint64_t n) {
        m_blocks = std::ceil(static_cast<double>(n) / b) + 1;
        m_size = n;

        std::vector<int64_t> fenwick_tree_data(1 + m_blocks * b, 0);
        std::copy(input, input + n, fenwick_tree_data.begin() + 1);

        uint64_t size = fenwick_tree_data.size();
        build(fenwick_tree_data.data(), 1, size - 1, size - 1);

        // for (auto x : fenwick_tree_data) std::cout << x << " ";
        // std::cout << std::endl;
        // std::cout << "===" << std::endl;

        std::vector<int64_t> node_data(b);
        m_data.resize(m_blocks * Node::bytes, 0);
        m_ptr = m_data.data();
        uint8_t* ptr = m_data.data();
        for (uint64_t i = 0, begin = 1; i != m_blocks; ++i, begin += b) {
            for (uint64_t k = 0; k != b; ++k) {
                node_data[k] = fenwick_tree_data[begin + k];
            }
            Node::build(node_data.data(), ptr);
            ptr += Node::bytes;
        }

        // std::cout << "===" << std::endl;
    }

    static std::string name() {
        return "fenwick_btree_" + Node::name();
    }

    uint64_t size() const {
        return m_size;
    }

    int64_t sum(uint64_t i) const {
        assert(i < size());

        static const uint64_t group_mask = (1ULL << log2_b) - 1;
        uint64_t groups = std::ceil(std::log2(m_size)) / log2_b;
        uint64_t power = pow(b, groups);
        uint64_t digit_mask = group_mask << (groups * log2_b);
        uint64_t offset = 0;

        i += 1;
        int64_t sum = 0;

        for (uint64_t group = groups; group != 0; --group) {
            uint64_t digit = (i & digit_mask) >> (group * log2_b);
            if (digit != 0) {
                offset += digit * power;
                uint64_t block = offset / b - 1;
                Node node(m_ptr + block * Node::bytes);
                sum += node.back();
            }
            power /= b;
            digit_mask >>= log2_b;
        }

        uint64_t digit = i & digit_mask;
        if (digit != 0) {
            offset += b;
            uint64_t block = offset / b - 1;
            Node node(m_ptr + block * Node::bytes);
            sum += node.sum(digit - 1);
        }

        return sum;
    }

    void update(uint64_t i, int64_t delta) {
        assert(i < size());

        static const uint64_t group_mask = (1ULL << log2_b) - 1;
        uint64_t groups = std::ceil(std::log2(m_size)) / log2_b;

        uint64_t block = i / b;
        Node node(m_ptr + block * Node::bytes);
        uint64_t digit = i & group_mask;
        node.update(digit, delta);
        if (digit != b - 1) node.update_back(delta);

        for (uint64_t power = b, group = 1, offset = block * b; group <= groups;
             power *= b, ++group) {
            i >>= log2_b;
            uint64_t digit = i & group_mask;
            uint64_t powers = b - digit - 1;
            for (uint64_t i = 0; i != powers; ++i) {
                offset += power;
                if (offset >= m_size) break;
                uint64_t block = offset / b;
                Node node(m_ptr + block * Node::bytes);
                node.update_back(delta);
            }
        }
    }

private:
    uint64_t m_blocks;
    uint64_t m_size;
    uint8_t* m_ptr;
    std::vector<uint8_t> m_data;

    // [l,r], not semi-open
    void build(int64_t* input, uint64_t l, uint64_t r, uint64_t size) {
        assert(r > l);
        int64_t sum = 0;
        for (uint64_t power = b, begin = l; power < r - l + 1; power *= b) {
            uint64_t powers = (b * (power + 1)) / power - 1;
            uint64_t end = 0;
            for (uint64_t i = 0; i != powers and end != size; ++i) {
                end = begin + power - 1;
                if (end <= r) {
                    for (uint64_t j = begin; j <= end; ++j) sum += input[j];
                    input[end] = sum;
                }
                if (end > size) end = size;
                build(input, begin, end - 1, size);
                begin = end + 1;
            }
        }
    }
};

int main() {
    static constexpr uint64_t sizes[] = {
        5,      9,       13,      25,      37,      251,     316,    398,
        501,    630,     794,     1000,    1258,    1584,    1995,   2511,
        3162,   3981,    5011,    6309,    7943,    10000,   12589,  15848,
        19952,  25118,   31622,   39810,   50118,   63095,   79432,  100000,
        125892, 158489,  199526,  251188,  316227,  398107,  501187, 630957,
        794328, 1000000, 1258925, 1584893, 1995262, 2511886, 3162277};

    // static const uint64_t n = 128;
    // std::vector<int64_t> x(n, 1);
    // // fenwick_btree<node64> fbt;
    // fenwick_btree<node4> fbt;
    // fbt.build(x.data(), x.size());
    // fbt.update(0, 1);
    // for (uint64_t i = 0; i != n; ++i) {
    //     std::cout << "sum(" << i << ") = ";
    //     std::cout << fbt.sum(i) << std::endl;
    // }

    for (uint64_t i = 0; i != sizeof(sizes) / sizeof(uint64_t); ++i) {
        test_tree<fenwick_btree<node4>>(sizes[i]);
    }

    for (uint64_t i = 0; i != sizeof(sizes) / sizeof(uint64_t); ++i) {
        test_tree<fenwick_btree<node64>>(sizes[i]);
    }

    return 0;
}