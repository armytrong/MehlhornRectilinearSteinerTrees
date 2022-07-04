//
// Created by jgier on 27.06.2022.
//

#ifndef HAUPTAUFGABE_2_DELAUNAYGRAPH_H
#define HAUPTAUFGABE_2_DELAUNAYGRAPH_H


#include <ostream>
#include <utility>
#include "../typedefs.h"
#include "../graph/Graph.h"


class DelaunayPriorityQueue;

class DelaunaySet;


class DelaunayGraph {
public:
    struct Node {
        Node(GridUnit x_coord, GridUnit y_coord, NodeId internal_id = -1, NodeId node_id = -1)
                : x_coord(x_coord), y_coord(y_coord), internal_id(internal_id), original_node_id(node_id) {}

        GridUnit x_coord;
        GridUnit y_coord;
        NodeId internal_id; // unique ascending identifier
        NodeId original_node_id;     // original node id

        bool operator==(Node const &other) const;

        bool operator<(Node const &other) const;

        [[nodiscard]] GridUnit distance(Node const &other) const;
    };

    DelaunayGraph() : _max_x(0), _max_y(0), _orig_max_x(0), _orig_max_y(0), _num_terminals(0) {}

    explicit DelaunayGraph(std::vector<Node> terminals);

    struct Edge {
        Node node_a;
        Node node_b;

        [[nodiscard]] bool operator<(Edge const &other) const;
        [[nodiscard]] bool operator==(Edge const &other) const;
    };

    void add_node(GridUnit x_coord, GridUnit y_coord, NodeId node_id = -1);
    void add_terminal(GridUnit x_coord, GridUnit y_coord, NodeId node_id = -1);
    void add_edge(Node terminal_a, Node terminal_b);

    void calculate_l1_delaunay_triangulation();
    void translate_from_1_to_infty_norm();
    void translate_from_infty_to_1_norm();

    void add_steiner_points();

    Graph export_graph();
    [[maybe_unused]] void print_as_postscript(std::ostream &os, const std::string &base_file_name);

    [[nodiscard]] NodeId num_nodes() const;

    [[nodiscard]] EdgeId num_edges() const;
    [[nodiscard]] std::vector<Node> const &nodes() const;
    [[nodiscard]] std::vector<Edge> const &edges() const;


private:

    static void update_inactivation_records(DelaunayPriorityQueue &X, DelaunaySet const &Y, Node terminal);

    static void translate_terminal_from_1_to_infty_norm(Node &t);

    static void translate_terminal_from_infty_to_1_norm(Node &t);

    std::vector<Node> _nodes;
    std::vector<Edge> _edges;
    GridUnit _max_x, _max_y, _orig_max_x, _orig_max_y;

    NodeId _num_terminals;
};


#endif //HAUPTAUFGABE_2_DELAUNAYGRAPH_H
