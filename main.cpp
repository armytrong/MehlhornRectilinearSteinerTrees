#include <iostream>
#include "STPFileParser.h"
#include "DelaunayGraphFactory.h"
#include "kruskal/kruskal_ind.h"
#include "graph/RectilinearGraph.h"

int main() {
    STPFileParser parser("rect_instance_20.stp");
    std::cout << std::endl << "---------Start instance----------" << std::endl;
    Graph graph = parser.create_graph();
    graph.print_graph(std::cout);
    std::cout << std::endl << "--------Delaunay result----------" << std::endl;
    Graph delaunay_graph = DelaunayGraphFactory::create_delaunay_graph(graph);
    delaunay_graph.print_graph(std::cout);
    std::cout << std::endl << "--------Kruskal result-----------" << std::endl;
    Graph kruskal_graph = kruskal(delaunay_graph);
    kruskal_graph.print_graph((std::cout));
    std::cout << std::endl << "------Rectilinear result---------" << std::endl;
    RectilinearGraph rectilinear_graph(kruskal_graph);
    rectilinear_graph.create_graph().print_graph(std::cout);


    return 0;
}
