#include <iostream>
#include "STPFileParser.h"
#include "dijkstra/DijkstraGraph.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    STPFileParser parser("rect_instance_19.stp");
    Graph graph = parser.create_graph();
    DijkstraGraph dijkstra_graph(graph);
    dijkstra_graph.dijkstras_algorithm(0);

    graph.print_graph(std::cout);
    return 0;
}
