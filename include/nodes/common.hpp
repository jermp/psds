#pragma once

namespace psds {

template <typename T>
void build_node_prefix_sums(T const* input, uint8_t* out, uint64_t segment_size,
                            uint64_t summary_bytes, uint64_t bytes) {
    std::fill(out, out + bytes, 0);
    int64_t* summary = reinterpret_cast<int64_t*>(out);
    int64_t* keys = reinterpret_cast<int64_t*>(out + summary_bytes);
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

}  // namespace psds