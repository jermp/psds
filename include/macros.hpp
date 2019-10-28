#pragma once

namespace psds {

#define SQARED(x) ((x) * (x))
#define CUBED(x) ((x) * (x) * (x))

#define SUM_H1                \
    Node node(m_ptr);         \
    int32_t s1 = node.sum(i); \
    return s1;

#define SUM_H2                                       \
    uint64_t child1 = i / Node::degree;              \
    uint64_t child2 = i % Node::degree;              \
    Node node1(m_ptr);                               \
    Node node2(m_ptr + (1 + child1) * Node::size);   \
    int32_t s1 = child1 ? node1.sum(child1 - 1) : 0; \
    int32_t s2 = node2.sum(child2);                  \
    return s1 + s2;

#define SUM_H3                                                   \
    uint64_t child1 = i / SQARED(Node::degree);                  \
    uint64_t child2 = (i % SQARED(Node::degree)) / Node::degree; \
    uint64_t child3 = (i % SQARED(Node::degree)) % Node::degree; \
    Node node1(m_ptr);                                           \
    Node node2(m_ptr + (1 + child1) * Node::size);               \
    Node node3(m_ptr + (1 + m_num_nodes_per_level[1] + child2 +  \
                        child1 * Node::degree) *                 \
                           Node::size);                          \
    int32_t s1 = child1 ? node1.sum(child1 - 1) : 0;             \
    int32_t s2 = child2 ? node2.sum(child2 - 1) : 0;             \
    int32_t s3 = node3.sum(child3);                              \
    return s1 + s2 + s3;

#define SUM_H4                                                            \
    uint64_t child1 = i / CUBED(Node::degree);                            \
    uint64_t child2 = (i % CUBED(Node::degree)) / SQARED(Node::degree);   \
    uint64_t child3 = (i % SQARED(Node::degree)) / Node::degree;          \
    uint64_t child4 = (i % SQARED(Node::degree)) % Node::degree;          \
    Node node1(m_ptr);                                                    \
    Node node2(m_ptr + (1 + child1) * Node::size);                        \
    Node node3(m_ptr + (1 + m_num_nodes_per_level[1] + child2 +           \
                        child1 * Node::degree) *                          \
                           Node::size);                                   \
    Node node4(m_ptr +                                                    \
               (1 + m_num_nodes_per_level[1] + m_num_nodes_per_level[2] + \
                child3 + child2 * Node::degree +                          \
                child1 * SQARED(Node::degree)) *                          \
                   Node::size);                                           \
    int32_t s1 = child1 ? node1.sum(child1 - 1) : 0;                      \
    int32_t s2 = child2 ? node2.sum(child2 - 1) : 0;                      \
    int32_t s3 = child3 ? node3.sum(child3 - 1) : 0;                      \
    int32_t s4 = node4.sum(child4);                                       \
    return s1 + s2 + s3 + s4;

#define UPDATE_H1          \
    Node node(m_ptr);      \
    node.update(i, delta); \
    return;

#define UPDATE_H2                                  \
    uint64_t child1 = i / Node::degree;            \
    uint64_t child2 = i % Node::degree;            \
    Node node1(m_ptr);                             \
    Node node2(m_ptr + (1 + child1) * Node::size); \
    node1.update(child1, delta);                   \
    node2.update(child2, delta);                   \
    return;

#define UPDATE_H3                                                \
    uint64_t child1 = i / SQARED(Node::degree);                  \
    uint64_t child2 = (i % SQARED(Node::degree)) / Node::degree; \
    uint64_t child3 = (i % SQARED(Node::degree)) % Node::degree; \
    Node node1(m_ptr);                                           \
    Node node2(m_ptr + (1 + child1) * Node::size);               \
    Node node3(m_ptr + (1 + m_num_nodes_per_level[1] + child2 +  \
                        child1 * Node::degree) *                 \
                           Node::size);                          \
    node1.update(child1, delta);                                 \
    node2.update(child2, delta);                                 \
    node3.update(child3, delta);                                 \
    return;

#define UPDATE_H4                                                         \
    uint64_t child1 = i / CUBED(Node::degree);                            \
    uint64_t child2 = (i % CUBED(Node::degree)) / SQARED(Node::degree);   \
    uint64_t child3 = (i % SQARED(Node::degree)) / Node::degree;          \
    uint64_t child4 = (i % SQARED(Node::degree)) % Node::degree;          \
    Node node1(m_ptr);                                                    \
    Node node2(m_ptr + (1 + child1) * Node::size);                        \
    Node node3(m_ptr + (1 + m_num_nodes_per_level[1] + child2 +           \
                        child1 * Node::degree) *                          \
                           Node::size);                                   \
    Node node4(m_ptr +                                                    \
               (1 + m_num_nodes_per_level[1] + m_num_nodes_per_level[2] + \
                child3 + child2 * Node::degree +                          \
                child1 * SQARED(Node::degree)) *                          \
                   Node::size);                                           \
    node1.update(child1, delta);                                          \
    node2.update(child2, delta);                                          \
    node3.update(child3, delta);                                          \
    node4.update(child4, delta);                                          \
    return;

}  // namespace psds