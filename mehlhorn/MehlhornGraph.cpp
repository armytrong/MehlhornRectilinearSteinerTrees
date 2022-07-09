//
// Created by jgier on 04.07.2022.
//

#include <numeric>
#include <cassert>
#include <optional>
#include "MehlhornGraph.h"
#include "dijkstra/DijkstraGraph.h"
#include "kruskal/DisjointSet.h"

void MehlhornGraph::calculate_mehlhorn_graph() {
    DijkstraGraph dijkstra_graph(_coordinate_graph);
    std::vector<NodeId> terminals(_coordinate_graph.num_terminals());
    std::iota(terminals.begin(), terminals.end(), 0);
    NodeId root_node = dijkstra_graph.add_node(terminals, std::vector<WeightType>(terminals.size(), 0));
    dijkstra_graph.dijkstras_algorithm(root_node);

    std::vector<Edge> candidate_mehlhorn_edges;
    for (auto const &edge: _coordinate_graph.edges()) {
        NodeId terminal_1 = dijkstra_graph[edge.node_a.internal_id].closest_terminal;
        NodeId terminal_2 = dijkstra_graph[edge.node_b.internal_id].closest_terminal;
        WeightType length = dijkstra_graph[edge.node_a.internal_id].distance_to_root + edge.length() +
                            dijkstra_graph[edge.node_b.internal_id].distance_to_root;
        if (terminal_1 != terminal_2) {
            candidate_mehlhorn_edges.push_back({terminal_1, terminal_2, length});
            auto &candidate_edge = candidate_mehlhorn_edges.back();

            add_path_to_edge(dijkstra_graph, edge, candidate_edge);
        }

    }

    bucket_sort(
            candidate_mehlhorn_edges,
            [](Edge const &edge) { return edge.smaller_node(); },
            _coordinate_graph.num_nodes()
    );

    bucket_sort(
            candidate_mehlhorn_edges,
            [](Edge const &edge) { return edge.larger_node(); },
            _coordinate_graph.num_nodes()
    );

    std::optional<Edge> current_mincost_edge = std::nullopt;

    for (auto const &candidate_edge: candidate_mehlhorn_edges) {
        if (not current_mincost_edge.has_value()) {
            current_mincost_edge = candidate_edge;
        } else if (not(
                current_mincost_edge->larger_node() == candidate_edge.larger_node() and
                current_mincost_edge->smaller_node() == candidate_edge.smaller_node()
        )) {
            _mehlhorn_edges.push_back(*current_mincost_edge);
            current_mincost_edge = candidate_edge;
        } else if (current_mincost_edge->length > candidate_edge.length) {
            current_mincost_edge = candidate_edge;
        }
    }
    _mehlhorn_edges.push_back(*current_mincost_edge);
}

void MehlhornGraph::add_path_to_edge(
        DijkstraGraph const &dijkstra_graph,
        DelaunayGraph::Edge const &edge,
        MehlhornGraph::Edge &candidate_edge) const {
    std::vector<NodeId> &path_to_node_a = candidate_edge.path;
    NodeId node_i = edge.node_a.internal_id;
    while (not _coordinate_graph.is_terminal_id(node_i)) {
        path_to_node_a.push_back(node_i);
        node_i = dijkstra_graph.predecessor(node_i);
    }
    path_to_node_a.push_back(node_i);
    std::reverse(path_to_node_a.begin(), path_to_node_a.end());

    std::vector<NodeId> path_to_node_b;
    node_i = edge.node_b.internal_id;
    while (not _coordinate_graph.is_terminal_id(node_i)) {
        path_to_node_a.push_back(node_i);
        node_i = dijkstra_graph.predecessor(node_i);
    }
    path_to_node_a.push_back(node_i);
}

void MehlhornGraph::bucket_sort(
        std::vector<Edge> &vector,
        TypeToNodeId<Edge> type_to_node_id,
        NodeId max_id) {
    std::vector<std::vector<Edge>> bucket_list;
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

void MehlhornGraph::kruskal_on_mehlhorn_edges() {
    std::vector<EdgeId> edge_ids(_mehlhorn_edges.size());
    std::iota(edge_ids.begin(), edge_ids.end(), 0);

    std::vector<Edge> included_edges;

    auto id_to_edge_projection = [this](auto const edge_id) { return _mehlhorn_edges[edge_id]; };

    std::sort(edge_ids.begin(), edge_ids.end(),
              [&id_to_edge_projection](EdgeId a, EdgeId b) {
                  return (id_to_edge_projection(a) < id_to_edge_projection(b));
              });
    DisjointSet disjoint_set;
    disjoint_set.make_sets(_coordinate_graph.num_nodes());
    for (auto edge_id: edge_ids) {
        auto const &edge = id_to_edge_projection(edge_id);

        if (!disjoint_set.set_equals(edge.head, edge.tail)) {
            // unite the sets of the parents of the nodes of edge
            disjoint_set.unite(edge.head, edge.tail);
            // add edge to the vector of included edges
            included_edges.push_back(edge);
        }
    }
    _mehlhorn_edges = included_edges;
}

CoordinateGraph MehlhornGraph::reconstruct_coord_graph_from_mehlhorn_edges() const {
    auto const &nodes = _coordinate_graph.nodes();
    CoordinateGraph ret_graph(nodes, _coordinate_graph.num_terminals());

    for (auto const &edge: _mehlhorn_edges) {
        for (int i = 1; i < edge.path.size(); i++) {
            ret_graph.add_edge(nodes[edge.path[i - 1]], nodes[edge.path[i]]);
        }
    }

    return ret_graph;
}

bool MehlhornGraph::Edge::operator<(const MehlhornGraph::Edge &other) const { return length < other.length; }
