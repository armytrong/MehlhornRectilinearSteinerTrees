#include <iostream>
#include <fstream>
#include "STPFileParser.h"
#include "MehlhornDelaunayGraphFactory.h"
#include "kruskal/kruskal_ind.h"
#include "graph/RectilinearGraph.h"

int main(int argc, char *argv[]) {
    STPFileParser parser("rect_instance_19.stp");
//    std::cout << std::endl << "--------Start instance-----------" << std::endl;
//    Graph graph = parser.create_graph();
//    //graph.print_graph(std::cout);
//    std::cout << std::endl << "--------Delaunay result----------" << std::endl;
//    Graph delaunay_graph = MehlhornDelaunayGraphFactory::create_delaunay_graph(graph);
//    //delaunay_graph.print_graph(std::cout);
//    std::cout << std::endl << "---------Kruskal result----------" << std::endl;
//    Graph kruskal_graph = kruskal(delaunay_graph);
//    //kruskal_graph.print_graph((std::cout));
//    std::cout << std::endl << "-------Rectilinear result--------" << std::endl;
//    RectilinearGraph rectilinear_graph(kruskal_graph);
//    auto graph_2 = rectilinear_graph.create_graph();
//    //graph_2.print_graph(std::cout);
//    std::cout << std::endl << "-------2nd Kruskal result--------" << std::endl;
//    auto kruskal_graph_2 = kruskal(graph_2);
//    kruskal_graph_2.print_graph(std::cout);
//
//    std::cout << std::endl << "----------PostScript-------------" << std::endl;
//    RectilinearGraph output_graph(kruskal_graph_2);
//    output_graph.print_as_postscript(std::cout, "../PostScript/template.ps");

    std::cout << std::endl << "--------Delaunay result----------" << std::endl;
    DelaunayGraph delaunay_graph = parser.create_delaunay_graph();
    delaunay_graph.translate_from_1_to_infty_norm();
    delaunay_graph.calculate();
    delaunay_graph.translate_from_infty_to_1_norm();
    delaunay_graph.print_as_postscript(std::cout, "../PostScript/template.ps");
    delaunay_graph.primitive_print(std::cout);
    auto graph = delaunay_graph.export_graph();
    graph.print_graph(std::cout);
    std::cout << std::endl << "---------Kruskal result----------" << std::endl;
    auto kruskal_graph = kruskal(graph);
    kruskal_graph.print_graph(std::cout);
    RectilinearGraph output_graph(kruskal_graph);
    std::ofstream file("../PostScript/test.ps");
    output_graph.print_as_postscript(file, "../PostScript/template.ps");
    file.close();


    return 0;
}
