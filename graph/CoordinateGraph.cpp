//
// Created by jgier on 05.07.2022.
//

#include "CoordinateGraph.h"
#include <fstream>
#include <valarray>
#include <cassert>

CoordinateGraph::CoordinateGraph(std::vector<Node> nodes, NodeId num_terminals) :
        _max_x(0), _max_y(0), _orig_max_x(0), _orig_max_y(0), _nodes(std::move(nodes)), _num_terminals(num_terminals) {
    for (auto const &terminal: _nodes) {
        _max_x = std::max(std::abs(terminal.x_coord), _max_x);
        _max_y = std::max(std::abs(terminal.y_coord), _max_y);
        _orig_max_x = _max_x;
        _orig_max_y = _max_y;
    }
}

Graph CoordinateGraph::export_graph() const {
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

void CoordinateGraph::print_as_postscript(std::ostream &os, const std::string &base_file_name) {
    std::ifstream base_file(base_file_name);
    assert(base_file);
    std::string line;

    os << base_file.rdbuf();
    base_file.close();

    auto grid_width = static_cast<GridUnit>(std::sqrt(std::max(_orig_max_x, _orig_max_y)));

    os << std::endl;
    os << "%%BeginSetup" << std::endl << std::endl;
    os << 0 << " " << (grid_width + 1) * (grid_width + 1) << " " << 0 << " " << (grid_width + 1) * (grid_width + 1)
       << " SetAxes" << std::endl << std::endl;

    os << num_terminals() << " DefineTerminals" << std::endl;
    for (auto terminal: _nodes) {
        if (is_terminal_id(terminal.internal_id)) {
            os << "\t" << terminal.x_coord + 1 << "\t" << terminal.y_coord + 1 << "\tDT" << std::endl;
        }
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

const std::vector<CoordinateGraph::Node> &CoordinateGraph::nodes() const { return _nodes; }

EdgeId CoordinateGraph::num_edges() const { return static_cast<EdgeId>(_edges.size()); }

const std::vector<CoordinateGraph::Edge> &CoordinateGraph::edges() const { return _edges; }

NodeId CoordinateGraph::num_nodes() const { return static_cast<NodeId>(_nodes.size()); }

NodeId CoordinateGraph::num_terminals() const { return _num_terminals; }

bool CoordinateGraph::is_terminal_id(NodeId node_id) const { return node_id < num_terminals(); }

void CoordinateGraph::add_edge(CoordinateGraph::Node node_a, CoordinateGraph::Node node_b) {
    _edges.push_back({node_a, node_b});
}

void CoordinateGraph::add_node(GridUnit x_coord, GridUnit y_coord) {
    _nodes.emplace_back(x_coord, y_coord, _nodes.size());
    _max_x = std::max(_max_x, std::abs(x_coord));
    _max_y = std::max(_max_y, std::abs(y_coord));
    _orig_max_x = _max_x;
    _orig_max_y = _max_y;
}

void CoordinateGraph::add_terminal(GridUnit x_coord, GridUnit y_coord) {
    if (_nodes.size() == _num_terminals) {
        _nodes.emplace_back(x_coord, y_coord, _nodes.size());
    } else {
        _nodes.emplace(_nodes.begin() + _num_terminals, x_coord, y_coord, _nodes.size());
    }
    _num_terminals += 1;
}