#include "kruskal_ind.h"
#include <ranges>
#include <numeric>
#include "disjoint_set_2.h"
#include "graph/Graph.h"
#include "delaunay/DelaunayGraph.h"

Graph kruskal(const Graph &input_graph) {
    Graph return_graph(input_graph.num_nodes(), input_graph.terminals());

    std::vector<EdgeId> edge_ids(input_graph.num_edges());
    std::iota(edge_ids.begin(), edge_ids.end(), 0);

    auto id_to_edge_projection = [&input_graph](auto const edge_id) { return input_graph.edges()[edge_id]; };

    std::sort(edge_ids.begin(), edge_ids.end(),
              [&id_to_edge_projection](EdgeId a, EdgeId b) {
                  return (id_to_edge_projection(a) < id_to_edge_projection(b));
              });
    Disjoint_Set disjoint_set;
    disjoint_set.make_sets(return_graph.num_nodes());
    for (auto edge_id: edge_ids) {
        auto const &edge = id_to_edge_projection(edge_id);

        if (!disjoint_set.set_equals(edge._head, edge._tail)) {
            // unite the sets of the parents of the nodes of edge
            disjoint_set.unite(edge._head, edge._tail);
            // add edge to the vector of included edges
            return_graph.add_edge(edge);
        }
    }

    return return_graph;
}

DelaunayGraph kruskal(DelaunayGraph const &delaunay_graph) {
    DelaunayGraph return_graph(delaunay_graph.nodes());
    std::vector<EdgeId> edge_ids(delaunay_graph.num_edges());
    std::iota(edge_ids.begin(), edge_ids.end(), 0);

    auto id_to_edge_projection = [&delaunay_graph](auto const edge_id) { return delaunay_graph.edges()[edge_id]; };

    std::sort(edge_ids.begin(), edge_ids.end(),
              [&id_to_edge_projection](EdgeId a, EdgeId b) {
                  return (id_to_edge_projection(a) < id_to_edge_projection(b));
              });
    Disjoint_Set set;
    set.make_sets(return_graph.num_nodes());
    for (auto edge_id: edge_ids) {
        auto const &edge = id_to_edge_projection(edge_id);

        if (!set.set_equals(edge.node_a.internal_id, edge.node_b.internal_id)) {
            // unite the sets of the parents of the nodes of edge
            set.unite(edge.node_a.internal_id, edge.node_b.internal_id);
            // add edge to the vector of included edges
            return_graph.add_edge(edge.node_a, edge.node_b);
        }
    }

    return return_graph;
}