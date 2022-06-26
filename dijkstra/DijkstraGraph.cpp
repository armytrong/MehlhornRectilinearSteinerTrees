//
// Created by jgier on 29.04.2022.
//

#include <ranges>
#include <utility>
#include "DijkstraGraph.h"
#include "FibonacciHeap.h"
#include "StandardHeap.h"
#include <map>
#include <cassert>


DijkstraGraph::DijkstraGraph(Graph const &graph) : _calculation_finished(false) {
    _nodes = std::vector<Node>(graph.num_nodes());
    for (size_t i = 0; i < _nodes.size(); i++) {
        _nodes[i]._id = static_cast<NodeId>(i);
    }

    for (auto const &edge: graph.edges()) {
        _nodes[edge._head].neighbours.push_back(static_cast<NodeId>(edge._tail));
        _nodes[edge._head].weights.push_back(edge._weight);

        _nodes[edge._tail].neighbours.push_back(static_cast<NodeId>(edge._head));
        _nodes[edge._tail].weights.push_back(edge._weight);
    }
}

const DijkstraGraph::Node &DijkstraGraph::operator[](NodeId index) const {
    return _nodes[index];
}

void DijkstraGraph::dijkstras_algorithm(NodeId root_node_id) {

    // auto id_to_node_projection = [this](auto const node_id) { return _nodes[node_id]; };

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

    StandardHeap candidates;
//    FibonacciHeap candidates;

    candidates.insert(root_node_id, 0);

    root_node.distance_to_root = 0;

    while (!candidates.empty()) {
        NodeId node_v_id = candidates.extract_min();
        Node &node_v = _nodes[node_v_id];
        node_v.included = true;

        // for (auto const& node_w : node_v.neighbours | std::views::transform(id_to_node_projection))
        // Fände ich auch schöner so, aber CLion beschwert sich da irgendwie. Vielleicht habe ich da was falsch
        // konfiguriert.
        for (size_t i = 0; i < node_v.neighbours.size(); i++) {
            auto const node_w_id = node_v.neighbours[i];
            auto const edge_weight = node_v.weights[i];

            Node &node_w = _nodes[node_w_id];

            if (node_w.distance_to_root == -1) {

                // TODO: extract method
                node_w.predecessor = node_v_id;
                node_w.distance_to_root = node_v.distance_to_root + edge_weight;

                candidates.insert(node_w_id, node_w.distance_to_root);

                // If the root node is the direct predecessor of w, that means that
                // w must be a terminal, as the root was exclusively connected to the
                // terminal nodes.
                if (node_v_id == root_node_id) {
                    node_w.closest_terminal = node_w_id;
                }
                    // Otherwise w has the same closest Terminal as v, if not updated later.
                else {
                    node_w.closest_terminal = node_v.closest_terminal;
                }
            }
                // Otherwise, if the old distance label is larger, we update it and decrease the
                // key in the prioritiy queue
            else if (node_w.distance_to_root > node_v.distance_to_root + edge_weight) {

                // TODO: extract method
                node_w.distance_to_root = node_v.distance_to_root + edge_weight;
                candidates.decrease_key(node_w_id, node_w.distance_to_root);
                node_w.predecessor = node_v_id;

                // If the root node is the direct predecessor of w, that means that
                // w must be a terminal, as the root was exclusively connected to the
                // terminal nodes.
                // Actually, the true case should never be reached, because the edge-weights
                // from the root node are 0, but it is implemented for consistency.
                if (node_v_id == root_node_id) {
                    node_w.closest_terminal = node_w_id;
                }
                    // Otherwise w has the same closest Terminal as v, if not updated later.
                else {
                    node_w.closest_terminal = node_v.closest_terminal;
                }
            }
        }
    }
    _calculation_finished = true;
}

[[maybe_unused]] Graph DijkstraGraph::generate_output_graph() {
    std::vector<Graph::Edge> edges;
    std::vector<NodeId> terminals;

    for (auto const &node: _nodes) {
        if (node.predecessor != -1) {
            WeightType edge_weight = node.distance_to_root - _nodes[node.predecessor].distance_to_root;
            edges.emplace_back(static_cast<NodeId>(node._id), static_cast<NodeId>(node.predecessor),
                               edge_weight);
        }
    }

    return {static_cast<NodeId>(_nodes.size()), terminals, edges};
}

DijkstraGraph::Node &DijkstraGraph::operator[](NodeId index) {
    return _nodes[index];
}

NodeId DijkstraGraph::add_node(std::vector<NodeId> neighbours, std::vector<WeightType> weights) {
    auto id = static_cast<NodeId>(_nodes.size());
    _nodes.push_back({id, std::move(neighbours), std::move(weights)});
    _calculation_finished = false;
    return id;
}

bool DijkstraGraph::calculation_finished() const { return _calculation_finished; }




