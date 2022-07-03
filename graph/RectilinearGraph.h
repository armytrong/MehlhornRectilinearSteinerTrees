//
// Created by jgier on 26.06.2022.
//

#ifndef HAUPTAUFGABE_2_RECTILINEARGRAPH_H
#define HAUPTAUFGABE_2_RECTILINEARGRAPH_H


#include <optional>
#include <cmath>
#include <cassert>
#include "typedefs.h"
#include "Graph.h"

class RectilinearGraph {
public:
    explicit RectilinearGraph(NodeId num_nodes);

    explicit RectilinearGraph(NodeId num_nodes, std::vector<NodeId> terminals);

    explicit RectilinearGraph(Graph const &graph);

    [[nodiscard]] NodeId num_nodes() const;

    void add_any_edge(NodeId node_a, NodeId node_b, bool flipped = true);

    int num_edges() {
        int ret = 0;
        for (auto b: _horizontal_edges_after_node) {
            if (b) ret += 1;
        }
        for (auto b: _vertical_edges_above_node) {
            if (b) ret += 1;
        }
        return ret;
    }

    [[nodiscard]] Graph export_graph() const;

    void print_as_postscript(std::ostream &os, const std::string &base_file_name);

private:
    NodeId _grid_width;
    std::vector<NodeId> _terminals;
    std::vector<bool> _horizontal_edges_after_node;
    std::vector<bool> _vertical_edges_above_node;

    [[maybe_unused]] [[nodiscard]] std::optional<EdgeId>
    get_horizontal_connecting_edge(NodeId node_a, NodeId node_b) const;

    [[maybe_unused]] [[nodiscard]] std::optional<EdgeId>
    get_vertical_connecting_edge(NodeId node_a, NodeId node_b) const;

    [[nodiscard]] bool validate_node(NodeId node) const;

    static void swap_if_a_larger(NodeId &node_a, NodeId &node_b);

    [[nodiscard]] int x_coord(NodeId node) const;

    [[nodiscard]] int y_coord(NodeId node) const;
};


#endif //HAUPTAUFGABE_2_RECTILINEARGRAPH_H
