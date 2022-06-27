#include <iostream>
#include "STPFileParser.h"
#include "DelaunayGraphFactory.h"
#include "kruskal/kruskal_ind.h"
#include "graph/RectilinearGraph.h"

int main(int argc, char *argv[]) {
    STPFileParser parser("rect_instance_20.stp");
    std::cout << std::endl << "--------Start instance-----------" << std::endl;
    Graph graph = parser.create_graph();
    //graph.print_graph(std::cout);
    std::cout << std::endl << "--------Delaunay result----------" << std::endl;
    Graph delaunay_graph = DelaunayGraphFactory::create_delaunay_graph(graph);
    //delaunay_graph.print_graph(std::cout);
    std::cout << std::endl << "---------Kruskal result----------" << std::endl;
    Graph kruskal_graph = kruskal(delaunay_graph);
    //kruskal_graph.print_graph((std::cout));
    std::cout << std::endl << "-------Rectilinear result--------" << std::endl;
    RectilinearGraph rectilinear_graph(kruskal_graph);
    auto graph_2 = rectilinear_graph.create_graph();
    //graph_2.print_graph(std::cout);
    std::cout << std::endl << "-------2nd Kruskal result--------" << std::endl;
    auto kruskal_graph_2 = kruskal(graph_2);
    kruskal_graph_2.print_graph(std::cout);

    std::cout << std::endl << "----------PostScript-------------" << std::endl;
    RectilinearGraph output_graph(kruskal_graph_2);
    output_graph.print_as_postscript(std::cout, "../PostScript/template.ps");


    return 0;
}
