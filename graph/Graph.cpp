//
// Created by jgier on 29.04.2022.
//

#include <sstream>
#include <utility>
#include "Graph.h"

constexpr char FILE_HEADER[] = "33D32945 STP File, STP Format Version 1.0";


Graph::Edge::Edge(Graph::NodeId node_a, Graph::NodeId node_b, int weight) :
        _head(node_a),
        _tail(node_b),
        _weight(weight) {}

bool Graph::Edge::operator<(const Graph::Edge &other) const {
    if (_weight < other._weight)
        return true;
    return false;
}

Graph::NodeId Graph::num_nodes() const {
    return _num_nodes;
}

void Graph::print_graph(std::ostream outstream) {
    outstream << FILE_HEADER << std::endl;
    outstream << std::endl;
    outstream << "SECTION Graph" << std::endl;

    outstream << "Nodes " << _num_nodes << std::endl;
    outstream << "Edges " << _edges.size() << std::endl;
    for (const Edge &e: _edges) {
        outstream << "E " << e._tail + 1 << " " << e._head + 1 << " " << e._weight << std::endl;
    }

    outstream << "END" << std::endl;
    outstream << "EOF" << std::endl;
}

const std::vector<Graph::Edge> &Graph::edges() const {
    return _edges;
}

Graph::Graph(
        Graph::NodeId num_nodes,
        std::vector<NodeId> terminals,
        std::vector<Edge> edges
) :
        _num_nodes(num_nodes),
        _terminals(std::move(terminals)),
        _edges(std::move(edges)) {}
