//
// Created by jgier on 29.04.2022.
//

#include <ranges>
#include <utility>
#include "DijkstraGraph.h"
#include "../heap/FibonacciHeap.h"
#include "../heap/StandardHeap.h"
#include <map>
#include <cassert>


DijkstraGraph::DijkstraGraph(const CoordinateGraph &coordinate_graph) : _calculation_finished(false) {
    _nodes = std::vector<Node>(coordinate_graph.num_nodes());
    for (size_t i = 0; i < _nodes.size(); i++) {
        _nodes[i].id = static_cast<NodeId>(i);
    }

    for (auto const &edge: coordinate_graph.edges()) {
        _nodes[edge.node_a.internal_id].neighbours.push_back(edge.node_b.internal_id);
        _nodes[edge.node_a.internal_id].weights.push_back(edge.length());

        _nodes[edge.node_b.internal_id].neighbours.push_back(edge.node_a.internal_id);
        _nodes[edge.node_b.internal_id].weights.push_back(edge.length());
    }
}

const DijkstraGraph::Node &DijkstraGraph::operator[](NodeId index) const {
    return _nodes[index];
}

void DijkstraGraph::dijkstras_algorithm(NodeId root_node_id) {

    assert(root_node_id < _nodes.size() && "Root node must be in graph.");

    Node &root_node = _nodes[root_node_id];

    for (auto &node: _nodes) {
        node.closest_terminal = INVALID_NODE;
        node.distance_to_root = INVALID_WEIGHT;
        node.predecessor = INVALID_NODE;
    }

    StandardHeap candidates;
//    FibonacciHeap candidates;

    candidates.insert(root_node_id, 0);

    root_node.distance_to_root = 0;

    while (!candidates.empty()) {
        NodeId node_v_id = candidates.extract_min();
        Node &node_v = _nodes[node_v_id];

        // for (auto const& node_w : node_v.neighbours | std::views::transform(id_to_node_projection))
        // Fände ich auch schöner so, aber CLion beschwert sich da irgendwie. Vielleicht habe ich da was falsch
        // konfiguriert.
        for (size_t i = 0; i < node_v.neighbours.size(); i++) {
            auto const node_w_id = node_v.neighbours[i];
            auto const edge_weight = node_v.weights[i];

            Node &node_w = _nodes[node_w_id];

            if (node_w.distance_to_root == INVALID_WEIGHT) {

                node_w.predecessor = node_v_id;
                node_w.distance_to_root = node_v.distance_to_root + edge_weight;

                candidates.insert(node_w_id, node_w.distance_to_root);

                // If the root node is the direct predecessor of w, that means that
                // w must be a terminal, as the root was exclusively connected to the
                // terminal nodes.
                if (node_v_id == root_node_id) {
                    node_w.closest_terminal = node_w_id;
                }
                    // Otherwise w has the same closest Node as v, if not updated later.
                else {
                    node_w.closest_terminal = node_v.closest_terminal;
                }
            }
                // Otherwise, if the old distance label is larger, we update it and decrease the
                // key in the prioritiy queue
            else if (node_w.distance_to_root > node_v.distance_to_root + edge_weight) {

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
                    // Otherwise w has the same closest Node as v, if not updated later.
                else {
                    node_w.closest_terminal = node_v.closest_terminal;
                }
            }
        }
    }
    _calculation_finished = true;
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

NodeId DijkstraGraph::predecessor(NodeId node) const {
    assert(calculation_finished());
    return _nodes[node].predecessor;
}




