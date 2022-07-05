#include <iostream>
#include "STPFileParser.h"
#include "kruskal/kruskal_ind.h"
#include "graph/RectilinearGraph.h"
#include "ArgumentHandler.h"
#include "mehlhorn/MehlhornGraph.h"

int main(int argc, char *argv[]) {
    ArgumentHandler argument_handler(argc, argv);
    auto const &parms = argument_handler.parms();

    if (not parms.run) return 0;

    auto print_verbose = [& parms](std::string const &s) { if (parms.verbose) std::cout << s << std::endl; };

    DelaunayGraph delaunay_graph;
    try {
        STPFileParser parser(parms);
        print_verbose("input parsed");
        delaunay_graph = parser.create_delaunay_graph();
    } catch (std::invalid_argument const &e) {
        std::cerr << "Failed to read input. Error: " << e.what() << std::endl;
        return 1;
    }

    delaunay_graph.calculate_l1_delaunay_triangulation();
    delaunay_graph.add_steiner_points();
    delaunay_graph.calculate_l1_delaunay_triangulation();
    print_verbose("Delaunay triangulation complete.");


    MehlhornGraph mehlhorn_graph(delaunay_graph);
    mehlhorn_graph.calculate_mehlhorn_graph();
    print_verbose("Mehlhorn Graph calculated.");
//    std::ofstream file("Output/output.ps");
//    delaunay_graph.print_as_postscript(file, "PostScript/template.ps");
//    file.close();
    mehlhorn_graph.kruskal_on_mehlhorn_edges();
    print_verbose("first kruskal complete");

    auto coordinate_graph = mehlhorn_graph.reconstruct_coord_graph_from_mehlhorn_edges();
    coordinate_graph.kruskal();
    print_verbose("second kruskal complete");

    coordinate_graph.l_shape_flipping();
    print_verbose("l-shape-flipping complete");

    coordinate_graph.reduce_nodes();

    std::ofstream file("Output/output.ps");
    coordinate_graph.print_as_postscript(file, "PostScript/template.ps");
    file.close();
//    RectilinearGraph rect_graph(kruskal_graph.export_graph());
//    print_verbose("constructed rectiliear graph");
//
//    auto graph2 = rect_graph.export_graph();
//    auto kruskal_graph2 = kruskal(graph2);
//    print_verbose("second kruskal complete");
//
//    std::cout << std::endl;
//
//    std::ofstream file(parms.output_filename);
//    std::ostream &ostream = (parms.output_filename.empty() or not file) ? std::cout : file;
//    if (not file and not parms.output_filename.empty()) {
//        std::cerr << "unable to open output file, falling back to standard output" << std::endl;
//    }
//
//    if (parms.output_format == OutputFormat::STP) {
//        kruskal_graph2.print_graph(ostream);
//    } else if (parms.output_format == OutputFormat::PS) {
//        RectilinearGraph output_graph(kruskal_graph2);
//        output_graph.print_as_postscript(ostream, "PostScript/template.ps");
//    }
//    file.close();


    return 0;
}
