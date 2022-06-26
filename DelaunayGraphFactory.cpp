//
// Created by jgier on 20.06.2022.
//

#include <cassert>
#include <optional>
#include "DelaunayGraphFactory.h"

void
DelaunayGraphFactory::bucket_sort(
        std::vector<CandidateEdge> &vector,
        DelaunayGraphFactory::TypeToNodeId<CandidateEdge> type_to_node_id,
        NodeId max_id) {
    std::vector<std::vector<CandidateEdge>> bucket_list;
    bucket_list.resize(max_id);
    for (auto const &t: vector) {
        bucket_list[type_to_node_id(t)].emplace_back(t);
    }
    size_t orig_i = 0;
    for (auto const &bucket: bucket_list) {
        for (auto const &item: bucket) {
            vector[orig_i] = item;
            orig_i++;
        }
    }
}

Graph DelaunayGraphFactory::create_delaunay_graph(
        DijkstraGraph const &dijkstra_graph,
        Graph const &graph
) {
    assert(dijkstra_graph.calculation_finished());
    std::vector<CandidateEdge> candidate_edges;
    for (auto const &edge: graph.edges()) {
        NodeId terminal_1 = dijkstra_graph[edge._head].closest_terminal;
        NodeId terminal_2 = dijkstra_graph[edge._tail].closest_terminal;
        // The distance to root equals the distance to the closest terminal
        WeightType weight = dijkstra_graph[edge._head].distance_to_root + edge._weight + dijkstra_graph[edge._tail]
                .distance_to_root;
        if (terminal_1 != terminal_2) {
            candidate_edges.emplace_back(terminal_1, terminal_2, weight);
        }
    }

    bucket_sort(
            candidate_edges,
            [](CandidateEdge const &candidate_edge) { return candidate_edge.smaller_terminal; },
            graph.num_nodes()
    );

    bucket_sort(
            candidate_edges,
            [](CandidateEdge const &candidate_edge) { return candidate_edge.larger_terminal; },
            graph.num_nodes()
    );

    std::vector<Graph::Edge> edges;

    std::optional<CandidateEdge> current_mincost_edge = std::nullopt;

    for (auto candidate_edge: candidate_edges) {
        if (not current_mincost_edge.has_value()) { current_mincost_edge = candidate_edge; }

        else if (not(
                current_mincost_edge->larger_terminal == candidate_edge.larger_terminal and
                current_mincost_edge->smaller_terminal == candidate_edge.smaller_terminal
        )) {
            edges.emplace_back(
                    current_mincost_edge->smaller_terminal,
                    current_mincost_edge->larger_terminal,
                    current_mincost_edge->weight
            );
            current_mincost_edge = candidate_edge;
        } else if (current_mincost_edge->weight >
                   candidate_edge.weight) { current_mincost_edge = candidate_edge; }
    }
    edges.emplace_back(
            current_mincost_edge->smaller_terminal,
            current_mincost_edge->larger_terminal,
            current_mincost_edge->weight
    );

    return {graph.num_nodes(), graph.terminals(), edges};
}

Graph DelaunayGraphFactory::create_delaunay_graph(const Graph &graph) {
    DijkstraGraph dijkstra_graph(graph);
    NodeId root_node = dijkstra_graph.add_node(graph.terminals(), std::vector<WeightType>(graph.terminals().size(), 0));
    dijkstra_graph.dijkstras_algorithm(root_node);
    return create_delaunay_graph(dijkstra_graph, graph);
}

DelaunayGraphFactory::CandidateEdge::CandidateEdge(NodeId terminal_1, NodeId terminal_2, WeightType weight) :
        smaller_terminal(std::min(terminal_1, terminal_2)),
        larger_terminal(std::max(terminal_1, terminal_2)),
        weight(weight) {}

