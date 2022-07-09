//
// Created by jgier on 05.07.2022.
//

#ifndef HAUPTAUFGABE_2_COORDINATEGRAPH_H
#define HAUPTAUFGABE_2_COORDINATEGRAPH_H


#include "typedefs.h"

class CoordinateGraph {
public:
    struct Node {
        Node(GridUnit x_coord, GridUnit y_coord, NodeId internal_id = INVALID_NODE)
                : x_coord(x_coord), y_coord(y_coord), internal_id(internal_id) {}

        GridUnit x_coord;
        GridUnit y_coord;
        NodeId internal_id; // unique ascending identifier

        bool operator==(Node const &other) const;

        bool operator<(Node const &other) const;

        [[nodiscard]] GridUnit distance(Node const &other) const;
    };

    CoordinateGraph() : _max_x(0), _max_y(0), _orig_max_x(0), _orig_max_y(0), _num_terminals(0) {}

    explicit CoordinateGraph(std::vector<Node> nodes, NodeId num_terminals);

    struct Edge {
        Node node_a;
        Node node_b;

        [[nodiscard]] bool operator<(Edge const &other) const;
        [[nodiscard]] bool operator==(Edge const &other) const;
        [[nodiscard]] WeightType length() const;
    };

    [[maybe_unused]] void print_as_postscript(std::ostream &os, const std::string &base_file_name);
    void print_as_stp(std::ostream &os);

    [[nodiscard]] NodeId num_nodes() const;
    [[nodiscard]] NodeId num_terminals() const;
    [[nodiscard]] EdgeId num_edges() const;
    [[nodiscard]] std::vector<Node> const &nodes() const;
    [[nodiscard]] std::vector<Edge> const &edges() const;

    [[nodiscard]] bool is_terminal_id(NodeId node_id) const;

    Node add_node(GridUnit x_coord, GridUnit y_coord);
    void add_terminal(GridUnit x_coord, GridUnit y_coord);
    void add_edge(Node node_a, Node node_b);

    void kruskal();
    void l_shape_flipping();
    void reduce_nodes();
protected:
    static GridUnit mid(GridUnit a, GridUnit b, GridUnit c);

    std::vector<Node> _nodes;
    std::vector<Edge> _edges;
    GridUnit _max_x, _max_y, _orig_max_x, _orig_max_y;

    NodeId _num_terminals;

};


#endif //HAUPTAUFGABE_2_COORDINATEGRAPH_H
