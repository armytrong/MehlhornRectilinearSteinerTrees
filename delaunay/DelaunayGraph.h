//
// Created by jgier on 27.06.2022.
//

#ifndef HAUPTAUFGABE_2_DELAUNAYGRAPH_H
#define HAUPTAUFGABE_2_DELAUNAYGRAPH_H


#include <ostream>
#include "../typedefs.h"
#include "../graph/Graph.h"


class DelaunayPriorityQueue;

class DelaunaySet;


class DelaunayGraph {
public:
    DelaunayGraph() : _max_x(0), _max_y(0) {}

    struct Terminal {
        Terminal(GridUnit x_coord, GridUnit y_coord, NodeId id = -1) : x_coord(x_coord), y_coord(y_coord), id(id) {}

        GridUnit x_coord;
        GridUnit y_coord;
        NodeId id;

        bool operator==(Terminal const &other) const { return x_coord == other.x_coord and y_coord == other.y_coord; }

        bool operator<(Terminal const &other) const {
            return x_coord < other.x_coord or (x_coord == other.x_coord and y_coord < other.y_coord);
        }
    };

    struct Edge {
        Terminal terminal_a;
        Terminal terminal_b;
    };

    void add_terminal(GridUnit x_coord, GridUnit y_coord, NodeId id = -1);
    void calculate();
    void primitive_print(std::ostream &os);

    void translate_from_1_to_infty_norm();
    void translate_from_infty_to_1_norm();


    Graph export_graph();

    void print_as_postscript(std::ostream &os, const std::string &base_file_name);

private:

    static void update_inactivation_records(DelaunayPriorityQueue &X, DelaunaySet const &Y, Terminal terminal);
    void translate_terminal_from_1_to_infty_norm(Terminal &t);
    void translate_terminal_from_infty_to_1_norm(Terminal &t);

    std::vector<Terminal> _terminals;
    std::vector<Edge> _edges;
    GridUnit _max_x, _max_y;
};


#endif //HAUPTAUFGABE_2_DELAUNAYGRAPH_H
