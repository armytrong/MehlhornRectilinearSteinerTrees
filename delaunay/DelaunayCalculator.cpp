//
// Created by jgier on 09.07.2022.
//

#include <map>
#include <cassert>
#include "DelaunayCalculator.h"


#include "DelaunayCalculator.h"
#include "DelaunayPriorityQueue.h"
#include "DelaunaySet.h"

CoordinateGraph DelaunayCalculator::l1_delaunay_triangulation(CoordinateGraph input_graph) {
    input_graph.translate_from_1_to_infty_norm();
    auto result = infty_delaunay_trianglulatoin(input_graph);
    result.translate_from_infty_to_1_norm();
    return result;
}

DelaunayCalculator::Node DelaunayCalculator::translate_node_from_1_to_infty_norm(
        const Node &node
) {
    return {node.x_coord + node.y_coord,
            node.x_coord - node.y_coord,
            node.internal_id};
}

DelaunayCalculator::Node DelaunayCalculator::translate_node_from_infty_to_1_norm(const DelaunayCalculator::Node &node) {
    GridUnit new_x = (node.x_coord + node.y_coord) / 2;
    return {new_x, node.x_coord - new_x, node.internal_id};
}

void
DelaunayCalculator::update_inactivation_records(DelaunayPriorityQueue &x_record_queue, const DelaunaySet &y_node_set,
                                                Node node) {
    auto predecessor = y_node_set.predecessor(node);
    auto successor = y_node_set.successor(node);
    if (predecessor->x_coord > node.x_coord and successor->x_coord >= node.x_coord) {
        if (x_record_queue.find_inactivation_record(node).has_value()) {
            x_record_queue.change_priority(node, node.x_coord + successor->y_coord - predecessor->y_coord);
        } else {
            x_record_queue.insert(node, node.x_coord + successor->y_coord - predecessor->y_coord,
                                  ActiveInactive::INACTIVE);
        }
    }
}

CoordinateGraph DelaunayCalculator::infty_delaunay_trianglulatoin(const CoordinateGraph &input_graph) {

    CoordinateGraph result(input_graph.nodes(), input_graph.num_terminals());

    DelaunayPriorityQueue x_record_queue(input_graph.max_x(), input_graph.max_y());
    for (auto const &node: input_graph.nodes()) {
        x_record_queue.insert(node, node.x_coord, ActiveInactive::ACTIVE);
    }
    DelaunaySet y_node_set;
    y_node_set.insert({-input_graph.max_x() - 1, -input_graph.max_y() - 1});
    y_node_set.insert({-input_graph.max_x() - 1, input_graph.max_y() + 1});

    std::map<Node, int> ages;
    int age_counter = 0;

    while (not x_record_queue.empty()) {
        auto const minimum_record = x_record_queue.extract_min();
        ages.insert(std::make_pair(minimum_record.terminal, age_counter));
        age_counter++;
        if (minimum_record.status == ActiveInactive::ACTIVE) {
            y_node_set.insert(minimum_record.terminal);
            auto const successor = y_node_set.successor(minimum_record.terminal);
            auto const predecessor = y_node_set.predecessor((minimum_record.terminal));
            assert(successor.has_value() and predecessor.has_value());
            if (successor->x_coord != -input_graph.max_x() - 1) {
                result.add_edge(minimum_record.terminal, *successor);
                update_inactivation_records(x_record_queue, y_node_set,
                                            ages[minimum_record.terminal] >= ages[*successor] ? *successor
                                                                                              : minimum_record.terminal);
            }
            if (predecessor->x_coord != -input_graph.max_x() - 1) {
                result.add_edge(minimum_record.terminal, *predecessor);
                update_inactivation_records(x_record_queue, y_node_set,
                                            ages[minimum_record.terminal] >= ages[*predecessor] ? *predecessor
                                                                                                : minimum_record.terminal);
            }
        } else {
            auto const predecessor = y_node_set.predecessor(minimum_record.terminal);
            y_node_set.erase(minimum_record.terminal);
            auto const successor = y_node_set.successor(*predecessor);
            if (predecessor->x_coord != -input_graph.max_x() - 1 and successor->x_coord != -input_graph.max_x() - 1) {
                result.add_edge(*predecessor, *successor);
            }
            update_inactivation_records(x_record_queue, y_node_set,
                                        ages[*predecessor] >= ages[*successor] ? *successor : *predecessor);
        }
    }
    return result;
}

void DelaunayCalculator::add_steiner_points(CoordinateGraph &coordinate_graph) {
    std::vector<std::vector<CoordinateGraph::Edge>> edges_by_node_id(coordinate_graph.num_nodes());

    for (auto const &edge: coordinate_graph.edges()) {
        edges_by_node_id[edge.node_a.internal_id].emplace_back(edge);
        edges_by_node_id[edge.node_b.internal_id].emplace_back(edge);
    }

    const NodeId orig_num_nodes = coordinate_graph.num_nodes();
    auto const &nodes = coordinate_graph.nodes();
    for (NodeId node_id = 0; node_id < orig_num_nodes; node_id++) {
        auto const node = nodes[node_id];
        auto const &edges = edges_by_node_id[node.internal_id];
        for (int j = 0; j < edges.size(); j++) {
            auto const &edge_a = edges[j];
            auto const node_a = edge_a.node_a == node ? edge_a.node_b : edge_a.node_a;
            for (int k = j + 1; k < edges.size(); k++) {
                auto const &edge_b = edges[k];
                auto const node_b = edge_b.node_a == node ? edge_b.node_b : edge_b.node_a;

                auto const center_x = CoordinateGraph::mid(node.x_coord, node_a.x_coord, node_b.x_coord);
                auto const center_y = CoordinateGraph::mid(node.y_coord, node_a.y_coord, node_b.y_coord);

                coordinate_graph.add_node(center_x, center_y);
            }
        }
    }
}
