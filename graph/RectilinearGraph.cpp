//
// Created by jgier on 26.06.2022.
//

#include "RectilinearGraph.h"
#include <iostream>
#include <fstream>

RectilinearGraph::RectilinearGraph(NodeId num_nodes) {
    auto const sqrt = static_cast<NodeId>(std::sqrt(num_nodes));
    assert(sqrt * sqrt == num_nodes && "The number of nodes in a rect graph needs to be an integer sqare");

    _grid_width = sqrt;

    _horizontal_edges_after_node = std::vector<bool>(num_nodes, false);
    _vertical_edges_above_node = std::vector<bool>(num_nodes, false);
}

RectilinearGraph::RectilinearGraph(NodeId num_nodes, std::vector<NodeId> terminals) : RectilinearGraph(num_nodes) {
    _terminals = std::move(terminals);
}

RectilinearGraph::RectilinearGraph(const Graph &graph) : RectilinearGraph(graph.num_nodes(), graph.terminals()) {
    for (auto const &edge: graph.edges()) {
        add_any_edge(edge._head, edge._tail);
    }
}

void RectilinearGraph::add_any_edge(NodeId node_a, NodeId node_b, bool flipped) {
    assert(validate_node(node_a) && validate_node(node_b));
    swap_if_a_larger(node_a, node_b);

    if (flipped) {
        while (node_b - node_a > node_b % _grid_width) {
            _vertical_edges_above_node[node_a] = true;
            node_a += _grid_width;
        }
    }

    while (node_a % _grid_width < node_b % _grid_width) {
        _horizontal_edges_after_node[node_a] = true;
        node_a += 1;
    }
    while (node_a % _grid_width > node_b % _grid_width) {
        _horizontal_edges_after_node[node_b] = true;
        node_b += 1;
    }

    if (not flipped) {
        while (node_b - node_a > node_b % _grid_width) {
            _vertical_edges_above_node[node_a] = true;
            node_a += _grid_width;
        }
    }
}

std::optional<EdgeId> RectilinearGraph::get_horizontal_connecting_edge(NodeId node_a, NodeId node_b) const {
    swap_if_a_larger(node_a, node_b);

    if (node_b - node_a == 1 and (node_a + 1) % _grid_width == 0) {
        return static_cast<EdgeId>(node_a);
    } else return std::nullopt;
}

std::optional<EdgeId> RectilinearGraph::get_vertical_connecting_edge(NodeId node_a, NodeId node_b) const {
    swap_if_a_larger(node_a, node_b);

    if (node_a % _grid_width == node_b % _grid_width) {
        return static_cast<EdgeId>(node_a);
    } else return std::nullopt;
}


void RectilinearGraph::swap_if_a_larger(NodeId &node_a, NodeId &node_b) {
    NodeId temp = node_a;
    node_a = std::min(node_a, node_b);
    node_b = std::max(temp, node_b);
}

NodeId RectilinearGraph::num_nodes() const {
    return _grid_width * _grid_width;
}

bool RectilinearGraph::validate_node(NodeId node) const {
    return node >= 0 and node < num_nodes();
}

Graph RectilinearGraph::export_graph() const {
    Graph result(num_nodes(), _terminals);
    for (NodeId i = 0; i < num_nodes(); i++) {
        if (_horizontal_edges_after_node[i]) {
            result.add_edge({i, i + 1, 1});
        }
        if (_vertical_edges_above_node[i]) {
            result.add_edge({i, i + _grid_width, 1});
        }
    }
    return result;
}

void RectilinearGraph::print_as_postscript(std::ostream &os, const std::string &base_file_name) {
    std::ifstream base_file(base_file_name);
    assert(base_file);
    std::string line;

    os << base_file.rdbuf();
    base_file.close();

    os << std::endl;
    os << "%%BeginSetup" << std::endl << std::endl;
    os << 0 << " " << _grid_width + 1 << " " << 0 << " " << _grid_width + 1 << " SetAxes" << std::endl << std::endl;

    os << _terminals.size() << " DefineTerminals" << std::endl;
    for (auto terminal_id: _terminals) {
        os << "\t" << x_coord(terminal_id) << "\t" << y_coord(terminal_id) << "\tDT" << std::endl;
    }

    os << std::endl << "%%EndSetup" << std::endl << std::endl;

    os << "%%Page: 1 1" << std::endl;
    os << "BeginPlot" << std::endl;
    os << "\tPlot_Terminals" << std::endl;
    for (int i = 0; i < num_nodes(); i++) {
        if (_vertical_edges_above_node[i]) {
            os << "\t" << x_coord(i) << "\t" << y_coord(i) << "\t" << x_coord(i) << "\t" << y_coord(i) + 1 << "\tS"
               << std::endl;
        }
        if (_horizontal_edges_after_node[i]) {
            os << "\t" << x_coord(i) << "\t" << y_coord(i) << "\t" << x_coord(i) + 1 << "\t" << y_coord(i) << "\tS"
               << std::endl;
        }
    }
    os << "  (Steiner Minimal Tree: " << _terminals.size() << "points, length=" << num_edges() << ")" << std::endl;
    os << "EndPlot" << std::endl;
}

int RectilinearGraph::x_coord(NodeId node) const {
    return node % _grid_width + 1;
}

int RectilinearGraph::y_coord(NodeId node) const {
    return static_cast<int>(node / _grid_width) + 1;
}
