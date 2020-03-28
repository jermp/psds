#pragma once

namespace psds {

#define SQARED(x) ((x) * (x))
#define CUBED(x) ((x) * (x) * (x))

#define SUM_H1                \
    Node node(m_ptr);         \
    int64_t s1 = node.sum(i); \
    return s1;

#define SUM_H2                                      \
    uint64_t child1 = i / Node::fanout;             \
    uint64_t child2 = i % Node::fanout;             \
    Node node1(m_ptr);                              \
    Node node2(m_ptr + (1 + child1) * Node::bytes); \
    int64_t s1 = node1.sum(child1);                 \
    int64_t s2 = node2.sum(child2);                 \
    return s1 + s2;

#define SUM_H3                                                   \
    uint64_t child1 = i / SQARED(Node::fanout);                  \
    uint64_t child2 = (i % SQARED(Node::fanout)) / Node::fanout; \
    uint64_t child3 = (i % SQARED(Node::fanout)) % Node::fanout; \
    Node node1(m_ptr);                                           \
    Node node2(m_ptr + (1 + child1) * Node::bytes);              \
    Node node3(m_ptr + (1 + m_num_nodes_per_level[1] + child2 +  \
                        child1 * Node::fanout) *                 \
                           Node::bytes);                         \
    int64_t s1 = node1.sum(child1);                              \
    int64_t s2 = node2.sum(child2);                              \
    int64_t s3 = node3.sum(child3);                              \
    return s1 + s2 + s3;

#define SUM_H4                                                            \
    uint64_t child1 = i / CUBED(Node::fanout);                            \
    uint64_t child2 = (i % CUBED(Node::fanout)) / SQARED(Node::fanout);   \
    uint64_t child3 = (i % SQARED(Node::fanout)) / Node::fanout;          \
    uint64_t child4 = (i % SQARED(Node::fanout)) % Node::fanout;          \
    Node node1(m_ptr);                                                    \
    Node node2(m_ptr + (1 + child1) * Node::bytes);                       \
    Node node3(m_ptr + (1 + m_num_nodes_per_level[1] + child2 +           \
                        child1 * Node::fanout) *                          \
                           Node::bytes);                                  \
    Node node4(m_ptr +                                                    \
               (1 + m_num_nodes_per_level[1] + m_num_nodes_per_level[2] + \
                child3 + child2 * Node::fanout +                          \
                child1 * SQARED(Node::fanout)) *                          \
                   Node::bytes);                                          \
    int64_t s1 = node1.sum(child1);                                       \
    int64_t s2 = node2.sum(child2);                                       \
    int64_t s3 = node3.sum(child3);                                       \
    int64_t s4 = node4.sum(child4);                                       \
    return s1 + s2 + s3 + s4;

#define UPDATE_H1          \
    Node node(m_ptr);      \
    node.update(i, delta); \
    return;

#define UPDATE_H2                                   \
    uint64_t child1 = i / Node::fanout;             \
    uint64_t child2 = i % Node::fanout;             \
    Node node1(m_ptr);                              \
    Node node2(m_ptr + (1 + child1) * Node::bytes); \
    node1.update(child1 + 1, delta);                \
    node2.update(child2, delta);                    \
    return;

#define UPDATE_H3                                                \
    uint64_t child1 = i / SQARED(Node::fanout);                  \
    uint64_t child2 = (i % SQARED(Node::fanout)) / Node::fanout; \
    uint64_t child3 = (i % SQARED(Node::fanout)) % Node::fanout; \
    Node node1(m_ptr);                                           \
    Node node2(m_ptr + (1 + child1) * Node::bytes);              \
    Node node3(m_ptr + (1 + m_num_nodes_per_level[1] + child2 +  \
                        child1 * Node::fanout) *                 \
                           Node::bytes);                         \
    node1.update(child1 + 1, delta);                             \
    node2.update(child2 + 1, delta);                             \
    node3.update(child3, delta);                                 \
    return;

#define UPDATE_H4                                                         \
    uint64_t child1 = i / CUBED(Node::fanout);                            \
    uint64_t child2 = (i % CUBED(Node::fanout)) / SQARED(Node::fanout);   \
    uint64_t child3 = (i % SQARED(Node::fanout)) / Node::fanout;          \
    uint64_t child4 = (i % SQARED(Node::fanout)) % Node::fanout;          \
    Node node1(m_ptr);                                                    \
    Node node2(m_ptr + (1 + child1) * Node::bytes);                       \
    Node node3(m_ptr + (1 + m_num_nodes_per_level[1] + child2 +           \
                        child1 * Node::fanout) *                          \
                           Node::bytes);                                  \
    Node node4(m_ptr +                                                    \
               (1 + m_num_nodes_per_level[1] + m_num_nodes_per_level[2] + \
                child3 + child2 * Node::fanout +                          \
                child1 * SQARED(Node::fanout)) *                          \
                   Node::bytes);                                          \
    node1.update(child1 + 1, delta);                                      \
    node2.update(child2 + 1, delta);                                      \
    node3.update(child3 + 1, delta);                                      \
    node4.update(child4, delta);                                          \
    return;

}  // namespace psds