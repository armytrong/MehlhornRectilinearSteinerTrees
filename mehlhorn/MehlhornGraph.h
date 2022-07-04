//
// Created by jgier on 04.07.2022.
//

#ifndef HAUPTAUFGABE_2_MEHLHORNGRAPH_H
#define HAUPTAUFGABE_2_MEHLHORNGRAPH_H


#include "delaunay/DelaunayGraph.h"
#include "dijkstra/DijkstraGraph.h"

class MehlhornGraph {
public:
    struct Edge {
        NodeId tail;
        NodeId head;
        WeightType length;
        std::vector<NodeId> path;

        [[nodiscard]] NodeId smaller_node() const { return tail < head ? tail : head; }

        [[nodiscard]] NodeId larger_node() const { return tail > head ? tail : head; }

        [[nodiscard]] bool operator<(Edge const &other) const;
    };

    explicit MehlhornGraph(DelaunayGraph const &delaunay_graph) : _delaunay_graph(delaunay_graph) {}

    void calculate_mehlhorn_graph();

    void kruskal_on_mehlhorn_edges();

private:

    template<typename T>
    using TypeToNodeId = NodeId (*)(T const &element);

    static void
    bucket_sort(std::vector<Edge> &vector, TypeToNodeId<Edge> type_to_node_id, NodeId max_id);

    DelaunayGraph const &_delaunay_graph;
    std::vector<Edge> _mehlhorn_edges;
    void
    add_path_to_edge(const DijkstraGraph &dijkstra_graph, const DelaunayGraph::Edge &edge, Edge &candidate_edge) const;
};


#endif //HAUPTAUFGABE_2_MEHLHORNGRAPH_H
