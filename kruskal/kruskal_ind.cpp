#include "kruskal_ind.h"
#include <ranges>
#include <numeric>
#include "disjoint_set_2.h"

Graph kruskal(const Graph &input_graph) {
    Graph return_graph(input_graph.num_nodes(), input_graph.terminals());

    // vector erzeugen und mit den Zahlen [ 0, edges.size() ) fuellen
    std::vector<EdgeId> edge_ids(input_graph.num_edges());
    std::iota(edge_ids.begin(), edge_ids.end(), 0);

    auto id_to_edge_projection = [&input_graph](auto const edge_id) { return input_graph.edges()[edge_id]; };

    std::sort(edge_ids.begin(), edge_ids.end(),
              [&id_to_edge_projection](EdgeId a, EdgeId b) {
                  return (id_to_edge_projection(a) < id_to_edge_projection(b));
              });
    // iteriere ueber die edges in der reihenfolge, wie sie im id-vector gespeichert sind
    // diese syntax range | adaptor, bedeutet, dass die range (hier edge_ids) erst durch die adaptor-funktion gejagt wird
    // (transform ruft die uebergebene funktion einmal mit jedem element auf und ist dann eine range ueber die return-values der funktion)
    // es gibt z.b. auch std::views::filter(unary_predicate), was dann nur die elemente durchlaesst, fuer die das unaere praedikat true zurueckgibt

    Disjoint_Set set;
    set.make_sets(return_graph.num_nodes());
    for (auto edge_id: edge_ids) {
        auto const &edge = id_to_edge_projection(edge_id);

        if (!set.set_equals(edge._head, edge._tail)) {
            // unite the sets of the parents of the nodes of edge
            set.unite(edge._head, edge._tail);
            // add edge to the vector of included edges
            return_graph.add_edge(edge);
        }
    }

    return return_graph;
}