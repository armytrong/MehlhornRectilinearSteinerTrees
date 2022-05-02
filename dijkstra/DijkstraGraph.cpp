//
// Created by jgier on 29.04.2022.
//

#include <numeric>
#include <ranges>
#include "DijkstraGraph.h"
#include "FibonacciHeap.h"


DijkstraGraph::DijkstraGraph(Graph const &graph) {
    _nodes = std::vector<Node>(graph.num_nodes());
    for (size_t i = 0; i < _nodes.size(); i++) {
        _nodes[i]._id = i;
    }

    for (auto const &edge: graph.edges()) {
        _nodes[edge._head].neighbours.push_back((edge._tail));
        _nodes[edge._head].weights.push_back(edge._weight);

        _nodes[edge._tail].neighbours.push_back((edge._head));
        _nodes[edge._tail].weights.push_back(edge._weight);
    }
}


void DijkstraGraph::dijkstras_algorithm(NodeId root_node_id) {

    auto id_to_node_projection = [this](auto const node_id) { return _nodes[node_id]; };

    if (root_node_id >= _nodes.size()) {
        throw std::invalid_argument("Root node not in graph.");
    }

    Node &root_node = _nodes[root_node_id];

    for (auto &node: _nodes) {
        node.closest_terminal = -1;
        node.distance_to_root = -1;
        node.predecessor = -1;
        node.included = false;
    }

    FibonacciHeap candidates;

    candidates.insert(root_node_id, 0);
    root_node.distance_to_root = 0;

    while (!candidates.empty()) {
        NodeId node_id = candidates.extract_min();
        std::cout << "adding node " << node_id << std::endl; // DEBUG
        Node &node = _nodes[node_id];
        node.included = true;

        for (Node const &potential_candidate_node: node.neighbours | std::views::transform(id_to_node_projection)) {
            std::cout << potential_candidate_node._id << std::endl;
        }
    }


}
