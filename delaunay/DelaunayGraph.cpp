//
// Created by jgier on 27.06.2022.
//

#include <map>
#include "DelaunayGraph.h"
#include "DelaunayPriorityQueue.h"
#include "DelaunaySet.h"
#include <fstream>
#include <cassert>
#include <complex>

void DelaunayGraph::add_node(GridUnit x_coord, GridUnit y_coord, NodeId node_id) {
    _nodes.emplace_back(x_coord, y_coord, _nodes.size(), node_id);
    _max_x = std::max(_max_x, std::abs(x_coord));
    _max_y = std::max(_max_y, std::abs(y_coord));
    _orig_max_x = _max_x;
    _orig_max_y = _max_y;
}

void DelaunayGraph::add_terminal(GridUnit x_coord, GridUnit y_coord, NodeId node_id) {
    if (_nodes.size() == _num_terminals) {
        _nodes.emplace_back(x_coord, y_coord, _nodes.size(), node_id);
    } else {
        _nodes.emplace(_nodes.begin() + _num_terminals, x_coord, y_coord, _nodes.size(), node_id);
    }
    _num_terminals += 1;
}

void DelaunayGraph::calculate_l1_delaunay_triangulation() {
    translate_from_1_to_infty_norm();

    DelaunayPriorityQueue X(_max_x, _max_y);
    for (auto const &terminal: _nodes) {
        X.insert(terminal, terminal.x_coord, ACTIVE);
    }
    DelaunaySet Y;
    Y.insert({-_max_x - 1, -_max_y - 1});
    Y.insert({-_max_x - 1, _max_y + 1});

    std::map<Node, int> ages;
    int age_counter = 0;

    while (not X.empty()) {
        auto P = X.extract_min();
        ages.insert(std::make_pair(P.terminal, age_counter));
        age_counter++;
        if (P.status == ACTIVE) {
            Y.insert(P.terminal);
            auto successor = Y.successor(P.terminal);
            auto predecessor = Y.predecessor((P.terminal));
            assert(successor.has_value() and predecessor.has_value());
            if (successor->x_coord != -_max_x - 1) {
                _edges.push_back({P.terminal, *successor});
                update_inactivation_records(X, Y, ages[P.terminal] >= ages[*successor] ? *successor : P.terminal);
            }
            if (predecessor->x_coord != -_max_x - 1) {
                _edges.push_back({P.terminal, *predecessor});
                update_inactivation_records(X, Y, ages[P.terminal] >= ages[*predecessor] ? *predecessor : P.terminal);
            }
        } else {
            auto q = Y.predecessor(P.terminal);
            Y.del(P.terminal);
            auto successor = Y.successor(*q);
            if (q->x_coord != -_max_x - 1 and successor->x_coord != -_max_x - 1) _edges.push_back({*q, *successor});
            update_inactivation_records(X, Y, ages[*q] >= ages[*successor] ? *successor : *q);
        }
    }

    translate_from_infty_to_1_norm();
}

void DelaunayGraph::update_inactivation_records(DelaunayPriorityQueue &X, const DelaunaySet &Y,
                                                DelaunayGraph::Node terminal) {
    auto r = Y.predecessor(terminal);
    auto q = Y.successor(terminal);
    if (r->x_coord > terminal.x_coord and q->x_coord >= terminal.x_coord) {
        if (X.find_inactivation_record(terminal).has_value()) {
            X.change_priority(terminal, terminal.x_coord + q->y_coord - r->y_coord);
        } else {
            X.insert(terminal, terminal.x_coord + q->y_coord - r->y_coord, INACTIVE);
        }
    }
}

Graph DelaunayGraph::export_graph() {
    NodeId sqrt = std::max(_orig_max_x, _orig_max_y) + 1;
    NodeId num_nodes = sqrt * sqrt;
    std::vector<NodeId> terminals;
    for (auto const &terminal: _nodes) {
        terminals.push_back(terminal.x_coord + terminal.y_coord * sqrt);
    }
    Graph ret{num_nodes, terminals};
    for (auto const &edge: _edges) {
        ret.add_edge({
                             edge.node_a.x_coord + edge.node_a.y_coord * sqrt,
                             edge.node_b.x_coord + edge.node_b.y_coord * sqrt,
                             edge.node_a.distance(edge.node_b)
                     });
    }

    return ret;
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

[[maybe_unused]] void DelaunayGraph::print_as_postscript(std::ostream &os, const std::string &base_file_name) {
    std::ifstream base_file(base_file_name);
    assert(base_file);
    std::string line;

    os << base_file.rdbuf();
    base_file.close();

    auto grid_width = static_cast<GridUnit>(std::sqrt(std::max(_orig_max_x, _orig_max_y)));

    os << std::endl;
    os << "%%BeginSetup" << std::endl << std::endl;
    os << 0 << " " << (grid_width + 1) * (grid_width + 1) << " " << 0 << " " << (grid_width + 1) * (grid_width + 1)
       << " SetAxes"
       <<
       std::endl <<
       std::endl;

    os << _nodes.size() << " DefineTerminals" << std::endl;
    for (auto terminal: _nodes) {
        os << "\t" << terminal.x_coord + 1 << "\t" << terminal.y_coord + 1 << "\tDT" << std::endl;
    }

    os << std::endl << "%%EndSetup" << std::endl << std::endl;

    os << "%%Page: 1 1" << std::endl;
    os << "BeginPlot" << std::endl;
    os << "\tPlot_Terminals" << std::endl;
    for (auto const &edge: _edges) {
        os << "\t" << edge.node_a.x_coord + 1
           << "\t" << edge.node_a.y_coord + 1
           << "\t" << edge.node_b.x_coord + 1
           << "\t" << edge.node_b.y_coord + 1 << "\tS" << std::endl;
    }

    //os << "  (Steiner Minimal Tree: " << _terminals.size() << "points, length=" << grid_width * grid_widht << ")" <<
    //std::endl;
    os << "EndPlot" << std::endl;
}

const std::vector<DelaunayGraph::Node> &DelaunayGraph::nodes() const { return _nodes; }

EdgeId DelaunayGraph::num_edges() const { return static_cast<EdgeId>(_edges.size()); }

const std::vector<DelaunayGraph::Edge> &DelaunayGraph::edges() const { return _edges; }

NodeId DelaunayGraph::num_nodes() const { return static_cast<NodeId>(_nodes.size()); }

void DelaunayGraph::add_edge(DelaunayGraph::Node terminal_a, DelaunayGraph::Node terminal_b) {
    _edges.push_back({terminal_a, terminal_b});
}

DelaunayGraph::DelaunayGraph(std::vector<Node> terminals) :
        _max_x(0), _max_y(0), _orig_max_x(0), _orig_max_y(0), _nodes(std::move(terminals)) {
    for (auto const &terminal: _nodes) {
        _max_x = std::max(std::abs(terminal.x_coord), _max_x);
        _max_y = std::max(std::abs(terminal.y_coord), _max_y);
        _orig_max_x = _max_x;
        _orig_max_y = _max_y;
    }
    _num_terminals = static_cast<NodeId>(_nodes.size());
}

void DelaunayGraph::add_steiner_points() {
    std::vector<std::vector<Edge>> edges_by_node_id(num_nodes());

    for (auto const &edge: edges()) {
        edges_by_node_id[edge.node_a.internal_id].emplace_back(edge);
        edges_by_node_id[edge.node_b.internal_id].emplace_back(edge);
    }

    auto const mid = [](GridUnit a, GridUnit b, GridUnit c) {
        return std::max(
                std::max(std::min(a, b), std::min(a, c)),
                std::min(b, c));
    };

    const NodeId orig_num_nodes = num_nodes();
    for (NodeId node_id = 0; node_id < orig_num_nodes; node_id++) {
        auto const &node = _nodes[node_id];
        auto const &edges = edges_by_node_id[node.internal_id];
        for (int j = 0; j < edges.size(); j++) {
            auto const &edge_a = edges[j];
            auto const &node_a = edge_a.node_a == node ? edge_a.node_b : edge_a.node_a;
            for (int k = j + 1; k < edges.size(); k++) {
                auto const &edge_b = edges[k];
                auto const &node_b = edge_b.node_a == node ? edge_b.node_b : edge_b.node_a;

                auto const center_x = mid(node.x_coord, node_a.x_coord, node_b.x_coord);
                auto const center_y = mid(node.y_coord, node_a.y_coord, node_b.y_coord);

                add_node(center_x, center_y);
            }
        }
    }
}

bool DelaunayGraph::Node::operator==(const DelaunayGraph::Node &other) const {
    return x_coord == other.x_coord and y_coord == other.y_coord;
}

bool DelaunayGraph::Node::operator<(const DelaunayGraph::Node &other) const {
    return x_coord < other.x_coord or (x_coord == other.x_coord and y_coord < other.y_coord);
}

GridUnit DelaunayGraph::Node::distance(const DelaunayGraph::Node &other) const {
    return std::abs(x_coord - other.x_coord) + std::abs(y_coord - other.y_coord);
}

bool DelaunayGraph::Edge::operator<(const DelaunayGraph::Edge &other) const {
    return node_a.distance(node_b) < other.node_a.distance((other.node_b));
}

bool DelaunayGraph::Edge::operator==(Edge const &other) const {
    return node_a == other.node_a and node_b == other.node_b;
}
