//
// Created by jgier on 29.04.2022.
//

#ifndef HAUPTAUFGABE_2_GRAPH_H
#define HAUPTAUFGABE_2_GRAPH_H

#include <vector>
#include <istream>
#include "../typedefs.h"

class Graph {
public:

    struct Edge {
        Edge(NodeId head, NodeId tail, int weight);

        bool operator<(Edge const &other) const;

        NodeId _head;
        NodeId _tail;
        int _weight;
    };

    explicit Graph(NodeId num_nodes) : _num_nodes(num_nodes) {}

    Graph(NodeId num_nodes, std::vector<NodeId> terminals) : _num_nodes(num_nodes), _terminals(std::move(terminals)) {}

    Graph(NodeId num_nodes, std::vector<NodeId> terminals, std::vector<Edge> edges);

    [[nodiscard]] NodeId virtual num_nodes() const;
    [[nodiscard]] EdgeId num_edges() const;

    [[nodiscard]] std::vector<Edge> const &edges() const;

    [[nodiscard]] std::vector<NodeId> const &terminals() const;

    void print_graph(std::basic_ostream<char> &outstream);

    void add_edge(Edge const &new_edge);


private:
    NodeId _num_nodes;
    std::vector<NodeId> _terminals;
    std::vector<Edge> _edges;

    std::vector<NodeId> nearest_terminal_to_node;
    std::vector<int> distance_node_to_nearest_terminal;
};


#endif //HAUPTAUFGABE_2_GRAPH_H
