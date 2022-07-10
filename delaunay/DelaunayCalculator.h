//
// Created by jgier on 09.07.2022.
//

#ifndef HAUPTAUFGABE_2_DELAUNAYCALCULATOR_H
#define HAUPTAUFGABE_2_DELAUNAYCALCULATOR_H

#include "graph/CoordinateGraph.h"
#include "typedefs.h"

class DelaunayPriorityQueue;

class DelaunaySet;


class DelaunayCalculator {
public:
    using Node = CoordinateGraph::Node;
    static CoordinateGraph l1_delaunay_triangulation(CoordinateGraph input_graph);
    static CoordinateGraph infty_delaunay_trianglulatoin(CoordinateGraph const &input_graph);
    static void add_steiner_points(CoordinateGraph &coordinate_graph);
private:
    static Node translate_node_from_1_to_infty_norm(const Node &node);
    static Node translate_node_from_infty_to_1_norm(const Node &node);
    static void update_inactivation_records(
            DelaunayPriorityQueue &x_record_queue,
            const DelaunaySet &y_node_set,
            Node node);
};

#endif //HAUPTAUFGABE_2_DELAUNAYCALCULATOR_H
