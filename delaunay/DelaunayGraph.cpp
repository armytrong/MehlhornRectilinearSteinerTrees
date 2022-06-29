//
// Created by jgier on 27.06.2022.
//

#include <map>
#include "DelaunayGraph.h"
#include "../heap/BinaryHeap.h"
#include "DelaunayPriorityQueue.h"
#include "DelaunaySet.h"
#include <fstream>
#include <cassert>
#include <complex>

void DelaunayGraph::add_terminal(GridUnit x_coord, GridUnit y_coord) {
    _terminals.emplace_back(x_coord, y_coord, _terminals.size());
    _max_x = std::max(_max_x, std::abs(x_coord));
    _max_y = std::max(_max_y, std::abs(y_coord));
    _orig_max_x = _max_x;
    _orig_max_y = _max_y;

}

void DelaunayGraph::calculate() {
    DelaunayPriorityQueue X(_max_x, _max_y);
    for (auto const &terminal: _terminals) {
        X.insert(terminal, terminal.x_coord, ACTIVE);
    }
    DelaunaySet Y;
    Y.insert({-_max_x - 1, -_max_y - 1});
    Y.insert({-_max_x - 1, _max_y + 1});

    std::map<Terminal, int> ages;
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
}

void DelaunayGraph::update_inactivation_records(DelaunayPriorityQueue &X, const DelaunaySet &Y,
                                                DelaunayGraph::Terminal terminal) {
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

void DelaunayGraph::primitive_print(std::ostream &os) {
    os << "Terminals: " << std::endl;
    for (auto const &terminal: _terminals) {
        os << "id: " << terminal.id << ", x: " << terminal.x_coord << ", y: " << terminal.y_coord << std::endl;
    }
    os << std::endl << "Edges: " << std::endl;
    for (auto const &edge: _edges) {
        os << edge.terminal_a.x_coord << " " << edge.terminal_a.y_coord << " -- " << edge.terminal_b.x_coord << " "
           << edge.terminal_b.y_coord << std::endl;
    }
}

Graph DelaunayGraph::export_graph() {
    NodeId sqrt = std::max(_orig_max_x, _orig_max_y) + 1;
    NodeId num_nodes = sqrt * sqrt;
    std::vector<NodeId> terminals;
    for (auto const &terminal: _terminals) {
        terminals.push_back(terminal.x_coord + terminal.y_coord * sqrt);
    }
    Graph ret{num_nodes, terminals};
    for (auto const &edge: _edges) {
        ret.add_edge({
                             edge.terminal_a.x_coord + edge.terminal_a.y_coord * sqrt,
                             edge.terminal_b.x_coord + edge.terminal_b.y_coord * sqrt,
                             std::abs(edge.terminal_a.x_coord - edge.terminal_b.x_coord)
                             + std::abs(edge.terminal_a.y_coord - edge.terminal_b.y_coord)
                     });
    }

    return ret;
}

void DelaunayGraph::translate_terminal_from_1_to_infty_norm(DelaunayGraph::Terminal &t) {
    GridUnit new_x = t.x_coord + t.y_coord;
    GridUnit new_y = t.x_coord - t.y_coord;
    t.x_coord = new_x;
    t.y_coord = new_y;
}

void DelaunayGraph::translate_terminal_from_infty_to_1_norm(DelaunayGraph::Terminal &t) {
    GridUnit new_x = (t.x_coord + t.y_coord) / 2;
    GridUnit new_y = t.x_coord - new_x;
    t.x_coord = new_x;
    t.y_coord = new_y;
}

void DelaunayGraph::translate_from_1_to_infty_norm() {
    for (auto &terminal: _terminals) {
        translate_terminal_from_1_to_infty_norm(terminal);
        _max_x = std::max(std::abs(terminal.x_coord), _max_x);
        _max_y = std::max(std::abs(terminal.y_coord), _max_y);
    }
    for (auto &edge: _edges) {
        translate_terminal_from_1_to_infty_norm(edge.terminal_a);
        translate_terminal_from_1_to_infty_norm(edge.terminal_b);
    }
}

void DelaunayGraph::translate_from_infty_to_1_norm() {
    for (auto &terminal: _terminals) {
        translate_terminal_from_infty_to_1_norm(terminal);
    }
    for (auto &edge: _edges) {
        translate_terminal_from_infty_to_1_norm(edge.terminal_a);
        translate_terminal_from_infty_to_1_norm(edge.terminal_b);
    }
}

void DelaunayGraph::print_as_postscript(std::ostream &os, const std::string &base_file_name) {
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

    os << _terminals.size() << " DefineTerminals" << std::endl;
    for (auto terminal: _terminals) {
        os << "\t" << terminal.x_coord + 1 << "\t" << terminal.y_coord + 1 << "\tDT" << std::endl;
    }

    os << std::endl << "%%EndSetup" << std::endl << std::endl;

    os << "%%Page: 1 1" << std::endl;
    os << "BeginPlot" << std::endl;
    os << "\tPlot_Terminals" << std::endl;
    for (auto const &edge: _edges) {
        os << "\t" << edge.terminal_a.x_coord + 1
           << "\t" << edge.terminal_a.y_coord + 1
           << "\t" << edge.terminal_b.x_coord + 1
           << "\t" << edge.terminal_b.y_coord + 1 << "\tS" << std::endl;
    }

    //os << "  (Steiner Minimal Tree: " << _terminals.size() << "points, length=" << grid_width * grid_widht << ")" <<
    //std::endl;
    os << "EndPlot" << std::endl;
}

const std::vector<DelaunayGraph::Terminal> &DelaunayGraph::terminals() const { return _terminals; }

EdgeId DelaunayGraph::num_edges() const { return static_cast<EdgeId>(_edges.size()); }

const std::vector<DelaunayGraph::Edge> &DelaunayGraph::edges() const { return _edges; }

NodeId DelaunayGraph::num_terminals() const { return _terminals.size(); }

void DelaunayGraph::add_edge(DelaunayGraph::Terminal terminal_a, DelaunayGraph::Terminal terminal_b) {
    _edges.push_back({terminal_a, terminal_b});
}

DelaunayGraph::DelaunayGraph(std::vector<Terminal> terminals) :
        _max_x(0), _max_y(0), _orig_max_x(0), _orig_max_y(0), _terminals(std::move(terminals)) {
    for (auto const &terminal: _terminals) {
        _max_x = std::max(std::abs(terminal.x_coord), _max_x);
        _max_y = std::max(std::abs(terminal.y_coord), _max_y);
        _orig_max_x = _max_x;
        _orig_max_y = _max_y;
    }
}

bool DelaunayGraph::Terminal::operator==(const DelaunayGraph::Terminal &other) const {
    return x_coord == other.x_coord and y_coord == other.y_coord;
}

bool DelaunayGraph::Terminal::operator<(const DelaunayGraph::Terminal &other) const {
    return x_coord < other.x_coord or (x_coord == other.x_coord and y_coord < other.y_coord);
}

GridUnit DelaunayGraph::Terminal::distance(const DelaunayGraph::Terminal &other) const {
    return std::abs(x_coord - other.x_coord) + std::abs(y_coord - other.y_coord);
}

bool DelaunayGraph::Edge::operator<(const DelaunayGraph::Edge &other) const {
    return terminal_a.distance(terminal_b) < other.terminal_a.distance((other.terminal_b));
}
