#pragma once

namespace psds {

#define SQARED(x) ((x) * (x))
#define CUBED(x) ((x) * (x) * (x))
#define BIQUADRATE(x) ((x) * (x) * (x) * (x))

#define NODE1 Node node1(m_ptr);

#define NODE2 \
    NODE1     \
    Node node2(m_ptr + (1 + child1) * Node::bytes);

#define NODE3                                                   \
    NODE2                                                       \
    Node node3(m_ptr + (1 + m_num_nodes_per_level[1] + child2 + \
                        child1 * Node::fanout) *                \
                           Node::bytes);

#define NODE4                                                             \
    NODE3                                                                 \
    Node node4(m_ptr +                                                    \
               (1 + m_num_nodes_per_level[1] + m_num_nodes_per_level[2] + \
                child3 + child2 * Node::fanout +                          \
                child1 * SQARED(Node::fanout)) *                          \
                   Node::bytes);

#define NODE5                                                               \
    NODE4                                                                   \
    Node node5(m_ptr +                                                      \
               (1 + m_num_nodes_per_level[1] + m_num_nodes_per_level[2] +   \
                m_num_nodes_per_level[3] + child4 + child3 * Node::fanout + \
                child2 * SQARED(Node::fanout) +                             \
                child1 * CUBED(Node::fanout)) *                             \
                   Node::bytes);

#define SUM_H1                 \
    NODE1                      \
    int64_t s1 = node1.sum(i); \
    return s1;

#define SUM_H2                          \
    uint64_t child1 = i / Node::fanout; \
    uint64_t child2 = i % Node::fanout; \
    NODE2                               \
    int64_t s1 = node1.sum(child1);     \
    int64_t s2 = node2.sum(child2);     \
    return s1 + s2;

#define SUM_H3                                                   \
    uint64_t child1 = i / SQARED(Node::fanout);                  \
    uint64_t child2 = (i % SQARED(Node::fanout)) / Node::fanout; \
    uint64_t child3 = (i % SQARED(Node::fanout)) % Node::fanout; \
    NODE3                                                        \
    int64_t s1 = node1.sum(child1);                              \
    int64_t s2 = node2.sum(child2);                              \
    int64_t s3 = node3.sum(child3);                              \
    return s1 + s2 + s3;

#define SUM_H4                                                          \
    uint64_t child1 = i / CUBED(Node::fanout);                          \
    uint64_t child2 = (i % CUBED(Node::fanout)) / SQARED(Node::fanout); \
    uint64_t child3 = (i % SQARED(Node::fanout)) / Node::fanout;        \
    uint64_t child4 = (i % SQARED(Node::fanout)) % Node::fanout;        \
    NODE4                                                               \
    int64_t s1 = node1.sum(child1);                                     \
    int64_t s2 = node2.sum(child2);                                     \
    int64_t s3 = node3.sum(child3);                                     \
    int64_t s4 = node4.sum(child4);                                     \
    return s1 + s2 + s3 + s4;

#define SUM_H5                                                              \
    uint64_t child1 = i / BIQUADRATE(Node::fanout);                         \
    uint64_t child2 = (i % BIQUADRATE(Node::fanout)) / CUBED(Node::fanout); \
    uint64_t child3 = (i % CUBED(Node::fanout)) / SQARED(Node::fanout);     \
    uint64_t child4 = (i % SQARED(Node::fanout)) / Node::fanout;            \
    uint64_t child5 = (i % SQARED(Node::fanout)) % Node::fanout;            \
    NODE5                                                                   \
    int64_t s1 = node1.sum(child1);                                         \
    int64_t s2 = node2.sum(child2);                                         \
    int64_t s3 = node3.sum(child3);                                         \
    int64_t s4 = node4.sum(child4);                                         \
    int64_t s5 = node5.sum(child5);                                         \
    return s1 + s2 + s3 + s4 + s5;

#define UPDATE_H1           \
    NODE1                   \
    node1.update(i, delta); \
    return;

#define UPDATE_H2                       \
    uint64_t child1 = i / Node::fanout; \
    uint64_t child2 = i % Node::fanout; \
    NODE2                               \
    node1.update(child1 + 1, delta);    \
    node2.update(child2, delta);        \
    return;

#define UPDATE_H3                                                \
    uint64_t child1 = i / SQARED(Node::fanout);                  \
    uint64_t child2 = (i % SQARED(Node::fanout)) / Node::fanout; \
    uint64_t child3 = (i % SQARED(Node::fanout)) % Node::fanout; \
    NODE3                                                        \
    node1.update(child1 + 1, delta);                             \
    node2.update(child2 + 1, delta);                             \
    node3.update(child3, delta);                                 \
    return;

#define UPDATE_H4                                                       \
    uint64_t child1 = i / CUBED(Node::fanout);                          \
    uint64_t child2 = (i % CUBED(Node::fanout)) / SQARED(Node::fanout); \
    uint64_t child3 = (i % SQARED(Node::fanout)) / Node::fanout;        \
    uint64_t child4 = (i % SQARED(Node::fanout)) % Node::fanout;        \
    NODE4                                                               \
    node1.update(child1 + 1, delta);                                    \
    node2.update(child2 + 1, delta);                                    \
    node3.update(child3 + 1, delta);                                    \
    node4.update(child4, delta);                                        \
    return;

#define UPDATE_H5                                                           \
    uint64_t child1 = i / BIQUADRATE(Node::fanout);                         \
    uint64_t child2 = (i % BIQUADRATE(Node::fanout)) / CUBED(Node::fanout); \
    uint64_t child3 = (i % CUBED(Node::fanout)) / SQARED(Node::fanout);     \
    uint64_t child4 = (i % SQARED(Node::fanout)) / Node::fanout;            \
    uint64_t child5 = (i % SQARED(Node::fanout)) % Node::fanout;            \
    NODE5                                                                   \
    node1.update(child1 + 1, delta);                                        \
    node2.update(child2 + 1, delta);                                        \
    node3.update(child3 + 1, delta);                                        \
    node4.update(child4 + 1, delta);                                        \
    node5.update(child5, delta);                                            \
    return;

}  // namespace psds