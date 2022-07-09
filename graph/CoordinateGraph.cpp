//
// Created by jgier on 05.07.2022.
//

#include "CoordinateGraph.h"
#include "kruskal/disjoint_set_2.h"
#include "STPFileParser.h"
#include <fstream>
#include <valarray>
#include <cassert>
#include <numeric>
#include <map>

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

    os << "\n";
    os << "%%BeginSetup" << "\n" << "\n";
    os << 0 << " " << (grid_width + 1) * (grid_width + 1) << " " << 0 << " " << (grid_width + 1) * (grid_width + 1)
       << " SetAxes" << "\n" << "\n";

    os << num_terminals() << " DefineTerminals" << "\n";
    for (auto terminal: _nodes) {
        if (is_terminal_id(terminal.internal_id)) {
            os << "\t" << terminal.x_coord + 1 << "\t" << terminal.y_coord + 1 << "\tDT" << "\n";
        }
    }

    os << "\n" << "%%EndSetup" << "\n" << "\n";

    os << "%%Page: 1 1" << "\n";
    os << "BeginPlot" << "\n";
    os << "\tPlot_Terminals" << "\n";
    for (auto const &edge: _edges) {
        os << "\t" << edge.node_a.x_coord + 1
           << "\t" << edge.node_a.y_coord + 1
           << "\t" << edge.node_b.x_coord + 1
           << "\t" << edge.node_b.y_coord + 1 << "\tS" << "\n";
    }

    //os << "  (Steiner Minimal Tree: " << _terminals.size() << "points, length=" << grid_width * grid_widht << ")" <<
    //"\n";
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

CoordinateGraph::Node CoordinateGraph::add_node(GridUnit x_coord, GridUnit y_coord) {
    _nodes.emplace_back(x_coord, y_coord, _nodes.size());
    _max_x = std::max(_max_x, std::abs(x_coord));
    _max_y = std::max(_max_y, std::abs(y_coord));
    _orig_max_x = _max_x;
    _orig_max_y = _max_y;
    return _nodes.back();
}

void CoordinateGraph::add_terminal(GridUnit x_coord, GridUnit y_coord) {
    if (_nodes.size() == _num_terminals) {
        _nodes.emplace_back(x_coord, y_coord, _nodes.size());
    } else {
        _nodes.emplace(_nodes.begin() + _num_terminals, x_coord, y_coord, _nodes.size());
    }
    _num_terminals += 1;
}

void CoordinateGraph::kruskal() {
    std::vector<EdgeId> edge_ids(num_edges());
    std::iota(edge_ids.begin(), edge_ids.end(), 0);

    std::vector<Edge> included_edges;

    auto id_to_edge_projection = [this](auto const edge_id) { return _edges[edge_id]; };

    std::sort(edge_ids.begin(), edge_ids.end(),
              [&id_to_edge_projection](EdgeId a, EdgeId b) {
                  return (id_to_edge_projection(a) < id_to_edge_projection(b));
              });
    Disjoint_Set set;
    set.make_sets(num_nodes());
    for (auto edge_id: edge_ids) {
        auto const &edge = id_to_edge_projection(edge_id);

        if (!set.set_equals(edge.node_a.internal_id, edge.node_b.internal_id)) {
            // unite the sets of the parents of the nodes of edge
            set.unite(edge.node_a.internal_id, edge.node_b.internal_id);
            // add edge to the vector of included edges
            included_edges.push_back(edge);
        }
    }
    _edges = included_edges;
}

void CoordinateGraph::l_shape_flipping() {

    auto const id_to_edge_projection = [this](EdgeId id) {
        assert(id < num_edges());
        return _edges[id];
    };

    std::vector<std::vector<EdgeId>> adjacence_vectors(num_nodes());


    std::vector<uint8_t> edge_deleted(num_edges(), false);

    for (EdgeId i = 0; i < num_edges(); i++) {
        Edge const &edge = _edges[i];
        adjacence_vectors[edge.node_a.internal_id].push_back(i);
        adjacence_vectors[edge.node_b.internal_id].push_back(i);
    }

    auto const orig_num_nodes = num_nodes();
    auto const orig_num_edges = num_edges();
    for (NodeId node_id = 0; node_id < orig_num_nodes; node_id++) {
        auto const node = _nodes[node_id];
        auto const &adjacent_edge_ids = adjacence_vectors[node_id];
        for (size_t j = 0; j < adjacent_edge_ids.size(); j++) {
            auto const edge_a_id = adjacent_edge_ids[j];
            if (edge_deleted[edge_a_id]) continue;
            auto const edge_a = id_to_edge_projection(edge_a_id);
            auto const node_a = edge_a.node_a == node ? edge_a.node_b : edge_a.node_a;

            for (size_t k = j + 1; k < adjacent_edge_ids.size(); k++) {
                auto const edge_b_id = adjacent_edge_ids[k];
                if (edge_deleted[edge_b_id]) continue;
                auto const edge_b = id_to_edge_projection(edge_b_id);
                auto const node_b = edge_b.node_a == node ? edge_b.node_b : edge_b.node_a;

                auto const old_edge_len = edge_a.length() + edge_b.length();
                auto const flipped_edge_len = (
                        std::max(std::max(node_a.x_coord, node_b.x_coord), node.x_coord) -
                        std::min(std::min(node_a.x_coord, node_b.x_coord), node.x_coord) +
                        std::max(std::max(node_a.y_coord, node_b.y_coord), node.y_coord) -
                        std::min(std::min(node_a.y_coord, node_b.y_coord), node.y_coord)
                );

                if (old_edge_len > flipped_edge_len) {
                    auto const steiner_point = add_node(mid(node_a.x_coord, node_b.x_coord, node.x_coord),
                                                        mid(node_a.y_coord, node_b.y_coord, node.y_coord));
                    add_edge(node, steiner_point);
                    add_edge(node_a, steiner_point);
                    add_edge(node_b, steiner_point);

                    edge_deleted[edge_a_id] = true;
                    edge_deleted[edge_b_id] = true;
                }
            }
        }
    }

    std::vector<Edge> new_edges;
    for (EdgeId i = 0; i < num_edges(); i++) {
        if (i >= orig_num_edges or (not edge_deleted[i])) {
            new_edges.push_back(_edges[i]);
        }
    }
    _edges = new_edges;
}


GridUnit CoordinateGraph::mid(GridUnit a, GridUnit b, GridUnit c) {
    return std::max(
            std::max(std::min(a, b), std::min(a, c)),
            std::min(b, c));
}

void CoordinateGraph::reduce_nodes() {
    NodeId orig_num_nodes = num_nodes();
    std::vector<NodeId> new_node_id(orig_num_nodes, -1);
    std::iota(new_node_id.begin(), new_node_id.begin() + num_terminals(), 0);
    NodeId node_id_counter = num_terminals();
    for (auto &edge: _edges) {
        if (new_node_id[edge.node_a.internal_id] == -1) {
            new_node_id[edge.node_a.internal_id] = node_id_counter;
            node_id_counter += 1;
        }
        if (new_node_id[edge.node_b.internal_id] == -1) {
            new_node_id[edge.node_b.internal_id] = node_id_counter;
            node_id_counter += 1;
        }
        edge.node_a.internal_id = new_node_id[edge.node_a.internal_id];
        edge.node_b.internal_id = new_node_id[edge.node_b.internal_id];
    }
    std::vector<Node> new_nodes(node_id_counter, {-1, -1, -1});
    for (int i = 0; i < new_node_id.size(); i++) {
        if (new_node_id[i] != -1) {
            auto const &old_node = _nodes[i];
            new_nodes[new_node_id[i]] = {old_node.x_coord, old_node.y_coord, new_node_id[i]};
        }
    }
    _nodes = new_nodes;
}

void CoordinateGraph::print_as_stp(std::ostream &outstream) {
    outstream << STPFileParser::FILE_HEADER << "\n";
    outstream << "\n";

    outstream << "SECTION Graph" << "\n";
    outstream << "Nodes " << num_nodes() << "\n";
    outstream << "Edges " << _edges.size() << "\n";
    for (auto const &edge: _edges) {
        outstream << "E " << edge.node_a.internal_id + 1
                  << " " << edge.node_b.internal_id + 1
                  << " " << edge.length() << "\n";
    }
    outstream << "END" << "\n";
    outstream << "\n";

    outstream << "SECTION Terminals" << "\n";
    outstream << "Terminals " << num_terminals() << "\n";
    for (int t = 0; t < num_terminals(); t++) {
        outstream << "T " << t + 1 << "\n";
    }
    outstream << "END" << "\n";
    outstream << "\n";

    outstream << "SECTION Coordinates" << "\n";
    for (auto const &node: _nodes) {
        outstream << "DD " << node.internal_id + 1
                  << " " << node.x_coord
                  << " " << node.y_coord << "\n";
    }
    outstream << "END" << "\n" << "\n";

    outstream << "EOF" << std::endl;
}


bool CoordinateGraph::Node::operator==(const CoordinateGraph::Node &other) const {
    return x_coord == other.x_coord and y_coord == other.y_coord;
}

bool CoordinateGraph::Node::operator<(const CoordinateGraph::Node &other) const {
    return x_coord < other.x_coord or (x_coord == other.x_coord and y_coord < other.y_coord);
}

GridUnit CoordinateGraph::Node::distance(const CoordinateGraph::Node &other) const {
    return std::abs(x_coord - other.x_coord) + std::abs(y_coord - other.y_coord);
}

bool CoordinateGraph::Edge::operator<(const CoordinateGraph::Edge &other) const {
    return node_a.distance(node_b) < other.node_a.distance((other.node_b));
}

bool CoordinateGraph::Edge::operator==(Edge const &other) const {
    return node_a == other.node_a and node_b == other.node_b;
}

WeightType CoordinateGraph::Edge::length() const {
    return std::abs(node_a.x_coord - node_b.x_coord) + std::abs(node_a.y_coord - node_b.y_coord);
}

