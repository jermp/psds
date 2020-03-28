#pragma once

namespace psds {

template <typename T>
void build_node_prefix_sums(T const* input, uint8_t* out, uint64_t segments,
                            uint64_t summary_bytes, uint64_t bytes) {
    std::fill(out, out + bytes, 0);
    int64_t* summary = reinterpret_cast<int64_t*>(out);
    int64_t* keys = reinterpret_cast<int64_t*>(out + summary_bytes);
    summary[0] = 0;
    for (uint64_t i = 0; i != segments; ++i) {
        uint64_t base = i * segments;
        keys[base] = input[base];
        for (uint64_t j = 1; j != segments; ++j) {
            keys[base + j] = keys[base + j - 1] + input[base + j];
        }
        if (i + 1 < segments) {
            summary[i + 1] = summary[i] + keys[(i + 1) * segments - 1];
        }
    }
}

}  // namespace psds