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
    DelaunayGraph() : _max_x(0), _max_y(0), _orig_max_x(0), _orig_max_y(0) {}

    struct Terminal {
        Terminal(GridUnit x_coord, GridUnit y_coord, NodeId internal_id = -1, NodeId node_id = -1)
                : x_coord(x_coord), y_coord(y_coord), internal_id(internal_id), node_id(node_id) {}

        GridUnit x_coord;
        GridUnit y_coord;
        NodeId internal_id; // unique ascending identifier
        NodeId node_id;     // original node id

        bool operator==(Terminal const &other) const;

        bool operator<(Terminal const &other) const;

        [[nodiscard]] GridUnit distance(Terminal const &other) const;
    };

    explicit DelaunayGraph(std::vector<Terminal> terminals);

    struct Edge {
        Terminal terminal_a;
        Terminal terminal_b;

        [[nodiscard]] bool operator<(Edge const &other) const;
    };

    void add_terminal(GridUnit x_coord, GridUnit y_coord, NodeId terminal_id = -1);

    void add_edge(Terminal terminal_a, Terminal terminal_b);

    void calculate();

    [[maybe_unused]] void primitive_print(std::ostream &os);

    void translate_from_1_to_infty_norm();

    void translate_from_infty_to_1_norm();

    Graph export_graph();

    [[maybe_unused]] void print_as_postscript(std::ostream &os, const std::string &base_file_name);

    [[nodiscard]] NodeId num_terminals() const;

    [[nodiscard]] EdgeId num_edges() const;

    [[nodiscard]] std::vector<Terminal> const &terminals() const;

    [[nodiscard]] std::vector<Edge> const &edges() const;


private:

    static void update_inactivation_records(DelaunayPriorityQueue &X, DelaunaySet const &Y, Terminal terminal);

    static void translate_terminal_from_1_to_infty_norm(Terminal &t);

    static void translate_terminal_from_infty_to_1_norm(Terminal &t);

    std::vector<Terminal> _terminals;
    std::vector<Edge> _edges;
    GridUnit _max_x, _max_y, _orig_max_x, _orig_max_y;
};


#endif //HAUPTAUFGABE_2_DELAUNAYGRAPH_H
