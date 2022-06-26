//
// Created by jgier on 29.04.2022.
//

#ifndef HAUPTAUFGABE_2_DIJKSTRAGRAPH_H
#define HAUPTAUFGABE_2_DIJKSTRAGRAPH_H

#include <iostream>
#include <vector>
#include "../typedefs.h"
#include "../graph/Graph.h"

class DijkstraGraph {
public:

    struct Node {
        NodeId _id = -1;
        std::vector<NodeId> neighbours;
        std::vector<WeightType> weights;
        NodeId predecessor = -1;
        NodeId closest_terminal = -1;
        WeightType distance_to_root = -1;
        bool included = false;
    };

    // TODO: make DijkstraGraph mutable

    explicit DijkstraGraph(Graph const &graph);

    Node &operator[](NodeId index);

    Node const &operator[](NodeId index) const;

    void dijkstras_algorithm(NodeId root_node_id);

    NodeId add_node(std::vector<NodeId> neighbours, std::vector<WeightType> weights);

    [[maybe_unused]] Graph generate_output_graph();

    [[nodiscard]] bool calculation_finished() const;

private:
    std::vector<Node> _nodes;
    bool _calculation_finished;
};


#endif //HAUPTAUFGABE_2_DIJKSTRAGRAPH_H
