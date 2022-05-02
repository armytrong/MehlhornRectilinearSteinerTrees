//
// Created by jgier on 29.04.2022.
//

#ifndef HAUPTAUFGABE_2_DIJKSTRAGRAPH_H
#define HAUPTAUFGABE_2_DIJKSTRAGRAPH_H

#include <iostream>
#include <vector>
#include "../graph/Graph.h"

class DijkstraGraph {
public:
    using NodeId = int;
    using WeightVal = int;

    struct Node {
        NodeId _id = -1;
        std::vector<NodeId> neighbours;
        std::vector<WeightVal> weights;
        NodeId predecessor = -1;
        NodeId closest_terminal = -1;
        WeightVal distance_to_root = -1;
        bool included = false;
    };

    explicit DijkstraGraph(Graph const &graph);

    Node &operator[](NodeId index) {
        return _nodes[index];
    }

    void dijkstras_algorithm(NodeId root_node_id);

private:
    std::vector<Node> _nodes;
};


#endif //HAUPTAUFGABE_2_DIJKSTRAGRAPH_H
