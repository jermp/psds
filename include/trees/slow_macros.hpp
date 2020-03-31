#pragma once

namespace psds {

#define SQARED(x) ((x) * (x))
#define CUBED(x) ((x) * (x) * (x))

#define SUM_H1                \
    Node node(m_ptr);         \
    int64_t s1 = node.sum(i); \
    return s1;

#define SUM_H2                                   \
    uint64_t child1 = i / Node::fanout;          \
    uint64_t child2 = i % Node::fanout;          \
    Node node;                                   \
    int64_t sum = 0;                             \
    node.at(m_ptr);                              \
    sum += node.sum(child1);                     \
    node.at(m_ptr + (1 + child1) * Node::bytes); \
    sum += node.sum(child2);                     \
    return sum;

#define SUM_H3                                                                \
    uint64_t child1 = i / SQARED(Node::fanout);                               \
    uint64_t child2 = (i % SQARED(Node::fanout)) / Node::fanout;              \
    uint64_t child3 = (i % SQARED(Node::fanout)) % Node::fanout;              \
    Node node;                                                                \
    int64_t sum = 0;                                                          \
    node.at(m_ptr);                                                           \
    sum += node.sum(child1);                                                  \
    node.at(m_ptr + (1 + child1) * Node::bytes);                              \
    sum += node.sum(child2);                                                  \
    node.at(m_ptr +                                                           \
            (1 + m_num_nodes_per_level[1] + child2 + child1 * Node::fanout) * \
                Node::bytes);                                                 \
    sum += node.sum(child3);                                                  \
    return sum;

#define SUM_H4                                                                 \
    uint64_t child1 = i / CUBED(Node::fanout);                                 \
    uint64_t child2 = (i % CUBED(Node::fanout)) / SQARED(Node::fanout);        \
    uint64_t child3 = (i % SQARED(Node::fanout)) / Node::fanout;               \
    uint64_t child4 = (i % SQARED(Node::fanout)) % Node::fanout;               \
    Node node;                                                                 \
    int64_t sum = 0;                                                           \
    node.at(m_ptr);                                                            \
    sum += node.sum(child1);                                                   \
    node.at(m_ptr + (1 + child1) * Node::bytes);                               \
    sum += node.sum(child2);                                                   \
    node.at(m_ptr +                                                            \
            (1 + m_num_nodes_per_level[1] + child2 + child1 * Node::fanout) *  \
                Node::bytes);                                                  \
    sum += node.sum(child3);                                                   \
    node.at(m_ptr +                                                            \
            (1 + m_num_nodes_per_level[1] + m_num_nodes_per_level[2] +         \
             child3 + child2 * Node::fanout + child1 * SQARED(Node::fanout)) * \
                Node::bytes);                                                  \
    sum += node.sum(child4);                                                   \
    return sum;

#define UPDATE_H1          \
    Node node(m_ptr);      \
    node.update(i, delta); \
    return;

#define UPDATE_H2                                \
    uint64_t child1 = i / Node::fanout;          \
    uint64_t child2 = i % Node::fanout;          \
    Node node;                                   \
    node.at(m_ptr);                              \
    node.update(child1 + 1, delta);              \
    node.at(m_ptr + (1 + child1) * Node::bytes); \
    node.update(child2, delta);                  \
    return;

#define UPDATE_H3                                                             \
    uint64_t child1 = i / SQARED(Node::fanout);                               \
    uint64_t child2 = (i % SQARED(Node::fanout)) / Node::fanout;              \
    uint64_t child3 = (i % SQARED(Node::fanout)) % Node::fanout;              \
    Node node;                                                                \
    node.at(m_ptr);                                                           \
    node.update(child1 + 1, delta);                                           \
    node.at(m_ptr + (1 + child1) * Node::bytes);                              \
    node.update(child2 + 1, delta);                                           \
    node.at(m_ptr +                                                           \
            (1 + m_num_nodes_per_level[1] + child2 + child1 * Node::fanout) * \
                Node::bytes);                                                 \
    node.update(child3, delta);                                               \
    return;

#define UPDATE_H4                                                              \
    uint64_t child1 = i / CUBED(Node::fanout);                                 \
    uint64_t child2 = (i % CUBED(Node::fanout)) / SQARED(Node::fanout);        \
    uint64_t child3 = (i % SQARED(Node::fanout)) / Node::fanout;               \
    uint64_t child4 = (i % SQARED(Node::fanout)) % Node::fanout;               \
    Node node;                                                                 \
    node.at(m_ptr);                                                            \
    node.update(child1 + 1, delta);                                            \
    node.at(m_ptr + (1 + child1) * Node::bytes);                               \
    node.update(child2 + 1, delta);                                            \
    node.at(m_ptr +                                                            \
            (1 + m_num_nodes_per_level[1] + child2 + child1 * Node::fanout) *  \
                Node::bytes);                                                  \
    node.update(child3 + 1, delta);                                            \
    node.at(m_ptr +                                                            \
            (1 + m_num_nodes_per_level[1] + m_num_nodes_per_level[2] +         \
             child3 + child2 * Node::fanout + child1 * SQARED(Node::fanout)) * \
                Node::bytes);                                                  \
    node.update(child4, delta);                                                \
    return;

}  // namespace psds