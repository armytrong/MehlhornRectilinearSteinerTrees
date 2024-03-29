//
// Created by jgier on 29.04.2022.
//

#ifndef HAUPTAUFGABE_2_DIJKSTRAGRAPH_H
#define HAUPTAUFGABE_2_DIJKSTRAGRAPH_H

#include <iostream>
#include <vector>
#include "typedefs.h"
#include "graph/Graph.h"
#include "delaunay/DelaunayGraph.h"

class DijkstraGraph {
public:

    struct Node {
        NodeId id = INVALID_NODE;
        std::vector<NodeId> neighbours;
        std::vector<WeightType> weights;
        NodeId predecessor = INVALID_NODE;
        NodeId closest_terminal = INVALID_NODE;
        WeightType distance_to_root = INVALID_WEIGHT;
    };


    explicit DijkstraGraph(Graph const &graph);
    explicit DijkstraGraph(CoordinateGraph const &coordinate_graph);

    Node &operator[](NodeId index);

    Node const &operator[](NodeId index) const;

    void dijkstras_algorithm(NodeId root_node_id);

    NodeId add_node(std::vector<NodeId> neighbours, std::vector<WeightType> weights);

    [[maybe_unused]] Graph generate_output_graph();

    [[nodiscard]] bool calculation_finished() const;

    [[nodiscard]] NodeId predecessor(NodeId node) const;

private:
    std::vector<Node> _nodes;
    bool _calculation_finished;
};


#endif //HAUPTAUFGABE_2_DIJKSTRAGRAPH_H
