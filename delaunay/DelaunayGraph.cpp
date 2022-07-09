//
// Created by jgier on 27.06.2022.
//

#include <map>
#include "DelaunayGraph.h"
#include "DelaunayPriorityQueue.h"
#include "DelaunaySet.h"
#include <fstream>
#include <cassert>


void DelaunayGraph::calculate_l1_delaunay_triangulation() {
    translate_from_1_to_infty_norm();
    _edges.clear();

    DelaunayPriorityQueue x_record_queue(_max_x, _max_y);
    for (auto const &terminal: _nodes) {
        x_record_queue.insert(terminal, terminal.x_coord, ActiveInactive::ACTIVE);
    }
    DelaunaySet y_node_set;
    y_node_set.insert({-_max_x - 1, -_max_y - 1});
    y_node_set.insert({-_max_x - 1, _max_y + 1});

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
            if (successor->x_coord != -_max_x - 1) {
                _edges.push_back({minimum_record.terminal, *successor});
                update_inactivation_records(x_record_queue, y_node_set,
                                            ages[minimum_record.terminal] >= ages[*successor] ? *successor
                                                                                              : minimum_record.terminal);
            }
            if (predecessor->x_coord != -_max_x - 1) {
                _edges.push_back({minimum_record.terminal, *predecessor});
                update_inactivation_records(x_record_queue, y_node_set,
                                            ages[minimum_record.terminal] >= ages[*predecessor] ? *predecessor
                                                                                                : minimum_record.terminal);
            }
        } else {
            auto const predecessor = y_node_set.predecessor(minimum_record.terminal);
            y_node_set.erase(minimum_record.terminal);
            auto const successor = y_node_set.successor(*predecessor);
            if (predecessor->x_coord != -_max_x - 1 and successor->x_coord != -_max_x - 1) {
                _edges.push_back({*predecessor, *successor});
            }
            update_inactivation_records(x_record_queue, y_node_set,
                                        ages[*predecessor] >= ages[*successor] ? *successor : *predecessor);
        }
    }

    translate_from_infty_to_1_norm();
}

void DelaunayGraph::update_inactivation_records(DelaunayPriorityQueue &x_record_queue, const DelaunaySet &y_node_set,
                                                DelaunayGraph::Node terminal) {
    auto predecessor = y_node_set.predecessor(terminal);
    auto successor = y_node_set.successor(terminal);
    if (predecessor->x_coord > terminal.x_coord and successor->x_coord >= terminal.x_coord) {
        if (x_record_queue.find_inactivation_record(terminal).has_value()) {
            x_record_queue.change_priority(terminal, terminal.x_coord + successor->y_coord - predecessor->y_coord);
        } else {
            x_record_queue.insert(terminal, terminal.x_coord + successor->y_coord - predecessor->y_coord,
                                  ActiveInactive::INACTIVE);
        }
    }
}


void DelaunayGraph::translate_terminal_from_1_to_infty_norm(DelaunayGraph::Node &t) {
    GridUnit new_x = t.x_coord + t.y_coord;
    GridUnit new_y = t.x_coord - t.y_coord;
    t.x_coord = new_x;
    t.y_coord = new_y;
}

void DelaunayGraph::translate_terminal_from_infty_to_1_norm(DelaunayGraph::Node &t) {
    GridUnit new_x = (t.x_coord + t.y_coord) / 2;
    GridUnit new_y = t.x_coord - new_x;
    t.x_coord = new_x;
    t.y_coord = new_y;
}

void DelaunayGraph::translate_from_1_to_infty_norm() {
    for (auto &terminal: _nodes) {
        translate_terminal_from_1_to_infty_norm(terminal);
        _max_x = std::max(std::abs(terminal.x_coord), _max_x);
        _max_y = std::max(std::abs(terminal.y_coord), _max_y);
    }
    for (auto &edge: _edges) {
        translate_terminal_from_1_to_infty_norm(edge.node_a);
        translate_terminal_from_1_to_infty_norm(edge.node_b);
    }
}

void DelaunayGraph::translate_from_infty_to_1_norm() {
    for (auto &terminal: _nodes) {
        translate_terminal_from_infty_to_1_norm(terminal);
    }
    for (auto &edge: _edges) {
        translate_terminal_from_infty_to_1_norm(edge.node_a);
        translate_terminal_from_infty_to_1_norm(edge.node_b);
    }
}


void DelaunayGraph::add_steiner_points() {
    std::vector<std::vector<Edge>> edges_by_node_id(num_nodes());

    for (auto const &edge: edges()) {
        edges_by_node_id[edge.node_a.internal_id].emplace_back(edge);
        edges_by_node_id[edge.node_b.internal_id].emplace_back(edge);
    }

    const NodeId orig_num_nodes = num_nodes();
    for (NodeId node_id = 0; node_id < orig_num_nodes; node_id++) {
        auto const node = _nodes[node_id];
        auto const &edges = edges_by_node_id[node.internal_id];
        for (int j = 0; j < edges.size(); j++) {
            auto const &edge_a = edges[j];
            auto const node_a = edge_a.node_a == node ? edge_a.node_b : edge_a.node_a;
            for (int k = j + 1; k < edges.size(); k++) {
                auto const &edge_b = edges[k];
                auto const node_b = edge_b.node_a == node ? edge_b.node_b : edge_b.node_a;

                auto const center_x = mid(node.x_coord, node_a.x_coord, node_b.x_coord);
                auto const center_y = mid(node.y_coord, node_a.y_coord, node_b.y_coord);

                add_node(center_x, center_y);
            }
        }
    }
}
