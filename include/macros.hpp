#pragma once

namespace dps {

#define SQARED(degree) ((degree) * (degree))
#define CUBED(degree) ((degree) * (degree) * (degree))

#define SUM_H1        \
    Node node(m_ptr); \
    int32_t s1 = node.sum(i);

#define SUM_H2(degree)                               \
    uint32_t child1 = i / degree;                    \
    uint32_t child2 = i % degree;                    \
    Node node1(m_ptr);                               \
    Node node2(m_ptr + (1 + child1) * Node::size);   \
    int32_t s1 = child1 ? node1.sum(child1 - 1) : 0; \
    int32_t s2 = node2.sum(child2);

#define SUM_H3(degree)                                                     \
    uint32_t child1 = i / SQARED(degree);                                  \
    uint32_t child2 = (i % SQARED(degree)) / degree;                       \
    uint32_t child3 = (i % SQARED(degree)) % degree;                       \
    Node node1(m_ptr);                                                     \
    Node node2(m_ptr + (1 + child1) * Node::size);                         \
    Node node3(m_ptr +                                                     \
               (1 + m_num_nodes_per_level[1] + child2 + child1 * degree) * \
                   Node::size);                                            \
    int32_t s1 = child1 ? node1.sum(child1 - 1) : 0;                       \
    int32_t s2 = child2 ? node2.sum(child2 - 1) : 0;                       \
    int32_t s3 = node3.sum(child3);

#define SUM_H4(degree)                                                     \
    uint32_t child1 = i / CUBED(degree);                                   \
    uint32_t child2 = (i % CUBED(degree)) / SQARED(degree);                \
    uint32_t child3 = (i % SQARED(degree)) / degree;                       \
    uint32_t child4 = (i % SQARED(degree)) % degree;                       \
    Node node1(m_ptr);                                                     \
    Node node2(m_ptr + (1 + child1) * Node::size);                         \
    Node node3(m_ptr +                                                     \
               (1 + m_num_nodes_per_level[1] + child2 + child1 * degree) * \
                   Node::size);                                            \
    Node node4(m_ptr +                                                     \
               (1 + m_num_nodes_per_level[1] + m_num_nodes_per_level[2] +  \
                child3 + child2 * degree + child1 * SQARED(degree)) *      \
                   Node::size);                                            \
    int32_t s1 = child1 ? node1.sum(child1 - 1) : 0;                       \
    int32_t s2 = child2 ? node2.sum(child2 - 1) : 0;                       \
    int32_t s3 = child3 ? node3.sum(child3 - 1) : 0;                       \
    int32_t s4 = node4.sum(child4);

#define UPDATE_H1     \
    Node node(m_ptr); \
    node.update(i, delta);

#define UPDATE_H2(degree)                          \
    uint32_t child1 = i / degree;                  \
    uint32_t child2 = i % degree;                  \
    Node node1(m_ptr);                             \
    Node node2(m_ptr + (1 + child1) * Node::size); \
    node1.update(child1, delta);                   \
    node2.update(child2, delta);

#define UPDATE_H3(degree)                                                  \
    uint32_t child1 = i / SQARED(degree);                                  \
    uint32_t child2 = (i % SQARED(degree)) / degree;                       \
    uint32_t child3 = (i % SQARED(degree)) % degree;                       \
    Node node1(m_ptr);                                                     \
    Node node2(m_ptr + (1 + child1) * Node::size);                         \
    Node node3(m_ptr +                                                     \
               (1 + m_num_nodes_per_level[1] + child2 + child1 * degree) * \
                   Node::size);                                            \
    node1.update(child1, delta);                                           \
    node2.update(child2, delta);                                           \
    node3.update(child3, delta);

#define UPDATE_H4(degree)                                                  \
    uint32_t child1 = i / CUBED(degree);                                   \
    uint32_t child2 = (i % CUBED(degree)) / SQARED(degree);                \
    uint32_t child3 = (i % SQARED(degree)) / degree;                       \
    uint32_t child4 = (i % SQARED(degree)) % degree;                       \
    Node node1(m_ptr);                                                     \
    Node node2(m_ptr + (1 + child1) * Node::size);                         \
    Node node3(m_ptr +                                                     \
               (1 + m_num_nodes_per_level[1] + child2 + child1 * degree) * \
                   Node::size);                                            \
    Node node4(m_ptr +                                                     \
               (1 + m_num_nodes_per_level[1] + m_num_nodes_per_level[2] +  \
                child3 + child2 * degree + child1 * SQARED(degree)) *      \
                   Node::size);                                            \
    node1.update(child1, delta);                                           \
    node2.update(child2, delta);                                           \
    node3.update(child3, delta);                                           \
    node4.update(child4, delta);

}  // namespace dps