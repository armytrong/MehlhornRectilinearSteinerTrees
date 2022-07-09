//
// Created by jgier on 29.04.2022.
//

#include <sstream>
#include <utility>
#include "Graph.h"

constexpr char FILE_HEADER[] = "33D32945 STP File, STP Format Version 1.0";


Graph::Edge::Edge(NodeId head, NodeId tail, int weight) : _head(head), _tail(tail), _weight(weight) {}

bool Graph::Edge::operator<(const Graph::Edge &other) const {
    if (_weight < other._weight)
        return true;
    return false;
}

NodeId Graph::num_nodes() const {
    return _num_nodes;
}

void Graph::print_graph(std::basic_ostream<char> &outstream) {
    outstream << FILE_HEADER << std::endl;
    outstream << std::endl;

    outstream << "SECTION Graph" << std::endl;
    outstream << "Nodes " << _num_nodes << std::endl;
    outstream << "Edges " << _edges.size() << std::endl;
    for (auto const &edge: _edges) {
        outstream << "E " << edge._tail + 1 << " " << edge._head + 1 << " " << edge._weight << std::endl;
    }
    outstream << "END" << std::endl;
    outstream << std::endl;

    outstream << "SECTION Terminals" << std::endl;
    outstream << "Terminals " << _terminals.size() << std::endl;
    for (auto const &terminal: _terminals) {
        outstream << "T " << terminal + 1 << std::endl;
    }
    outstream << "END" << std::endl;
    outstream << std::endl;

    outstream << "EOF" << std::endl;
}

const std::vector<Graph::Edge> &Graph::edges() const {
    return _edges;
}

Graph::Graph(
        NodeId num_nodes,
        std::vector<NodeId> terminals,
        std::vector<Edge> edges
) :
        _num_nodes(num_nodes),
        _terminals(std::move(terminals)),
        _edges(std::move(edges)) {}

const std::vector<NodeId> &Graph::terminals() const {
    return _terminals;
}

EdgeId Graph::num_edges() const {
    return static_cast<EdgeId>(_edges.size());
}

void Graph::add_edge(Graph::Edge const &new_edge) {
    if (new_edge._head < _num_nodes and new_edge._tail < _num_nodes) {
        _edges.push_back({new_edge});
    }
}
