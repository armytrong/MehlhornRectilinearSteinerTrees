//
// Created by jgier on 20.06.2022.
//

#ifndef HAUPTAUFGABE_2_DELAUNAYGRAPHFACTORY_H
#define HAUPTAUFGABE_2_DELAUNAYGRAPHFACTORY_H


#include "graph/Graph.h"
#include "dijkstra/DijkstraGraph.h"

class DelaunayGraphFactory {
public:


    static Graph create_delaunay_graph(Graph const &graph);

    static Graph create_delaunay_graph(DijkstraGraph const &dijkstra_graph, Graph const &graph);

private:
    struct CandidateEdge {
        CandidateEdge(NodeId terminal_1, NodeId terminal_2, WeightType weight);

        // structuring the edges with smaller and larger terminals makes using them easier, as edges between the same
        // nodes are grouped together in sorting.
        NodeId smaller_terminal;
        NodeId larger_terminal;
        WeightType weight;
    };

    template<typename T>
    using TypeToNodeId = NodeId (*)(T const &element);

    static void
    bucket_sort(std::vector<CandidateEdge> &vector, TypeToNodeId<CandidateEdge> type_to_node_id, NodeId max_id);
};


#endif //HAUPTAUFGABE_2_DELAUNAYGRAPHFACTORY_H
