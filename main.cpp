#include <iostream>
#include "STPFileParser.h"
#include "kruskal/kruskal_ind.h"
#include "graph/RectilinearGraph.h"
#include "ArgumentHandler.h"

int main(int argc, char *argv[]) {
    ArgumentHandler argument_handler(argc, argv);
    auto const &parms = argument_handler.parms();

    if (not parms.run) return 0;

    auto print_verbose = [& parms](std::string const &s) { if (parms.verbose) std::cout << s << std::endl; };

    STPFileParser parser(parms.input_filename, parms.instances_to_skip);
    print_verbose("input parsed");

    DelaunayGraph delaunay_graph = parser.create_delaunay_graph();
    delaunay_graph.translate_from_1_to_infty_norm();
    delaunay_graph.calculate();
    delaunay_graph.translate_from_infty_to_1_norm();
    print_verbose("Delaunay triangulation complete");

    auto kruskal_graph = kruskal(delaunay_graph);
    print_verbose("first kruskal complete");

    RectilinearGraph rect_graph(kruskal_graph.export_graph());
    print_verbose("constructed rectiliear graph");

    auto graph2 = rect_graph.export_graph();
    auto kruskal_graph2 = kruskal(graph2);
    print_verbose("second kruskal complete");

    std::cout << std::endl;

    std::ofstream file(parms.output_filename);
    std::ostream &ostream = (parms.output_filename.empty() or not file) ? std::cout : file;
    if (not file and not parms.output_filename.empty()) {
        std::cerr << "unable to open output file, falling back to standard output" << std::endl;
    }

    if (parms.output_format == OutputFormat::STP) {
        kruskal_graph2.print_graph(ostream);
    } else if (parms.output_format == OutputFormat::PS) {
        RectilinearGraph output_graph(kruskal_graph2);
        output_graph.print_as_postscript(ostream, "PostScript/template.ps");
    }
    file.close();


    return 0;
}
