//
// Created by jgier on 27.06.2022.
//

#ifndef HAUPTAUFGABE_2_DELAUNAYGRAPH_H
#define HAUPTAUFGABE_2_DELAUNAYGRAPH_H


#include <ostream>
#include <utility>
#include "typedefs.h"
#include "graph/Graph.h"
#include "graph/CoordinateGraph.h"


class DelaunayPriorityQueue;

class DelaunaySet;


class DelaunayGraph : public CoordinateGraph {
public:
    using super = CoordinateGraph;
    using Node = super::Node;
    using Edge = super::Edge;

    DelaunayGraph() : super() {}

    explicit DelaunayGraph(std::vector<Node> nodes, NodeId num_terminals) : super(std::move(nodes), num_terminals) {}


    void calculate_l1_delaunay_triangulation();
    void translate_from_1_to_infty_norm();
    void translate_from_infty_to_1_norm();

    void add_steiner_points();


private:

    static void
    update_inactivation_records(DelaunayPriorityQueue &x_record_queue, DelaunaySet const &y_node_set, Node terminal);

    static void translate_terminal_from_1_to_infty_norm(Node &t);

    static void translate_terminal_from_infty_to_1_norm(Node &t);


};


#endif //HAUPTAUFGABE_2_DELAUNAYGRAPH_H
