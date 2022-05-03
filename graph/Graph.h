//
// Created by jgier on 29.04.2022.
//

#ifndef HAUPTAUFGABE_2_GRAPH_H
#define HAUPTAUFGABE_2_GRAPH_H

#include <vector>
#include <istream>

class Graph {
public:
    using EdgeId = size_t;
    using NodeId = size_t;

    struct Edge {
        Edge(NodeId node_a, NodeId node_b, int weight);

        bool operator<(const Edge &other) const;

        NodeId _head;
        NodeId _tail;
        int _weight;
    };

    Graph(NodeId num_nodes, std::vector<NodeId> terminals, std::vector<Edge> edges);

    [[nodiscard]] NodeId num_nodes() const;

    [[nodiscard]] std::vector<Edge> const &edges() const;

    void print_graph(std::basic_ostream<char> &outstream);


private:
    NodeId _num_nodes;
    std::vector<NodeId> _terminals;
    std::vector<Edge> _edges;

    std::vector<NodeId> nearest_terminal_to_node;
    std::vector<int> distance_node_to_nearest_terminal;
};


#endif //HAUPTAUFGABE_2_GRAPH_H
