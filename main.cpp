#include <iostream>
#include "STPFileParser.h"
#include "ArgumentHandler.h"
#include "mehlhorn/MehlhornGraph.h"

int main(int argc, char *argv[]) {
    ArgumentHandler argument_handler(argc, argv);
    auto const &parms = argument_handler.parms();

    if (not parms.run) return 0;

    std::ifstream ifile(parms.input_filename);
    std::istream &istream = (parms.input_filename.empty() or not ifile) ? std::cin : ifile;
    if (not ifile and not parms.input_filename.empty()) {
        std::cerr << "unable to open input file, falling back to standard input" << std::endl;
    }

    std::ofstream ofile(parms.output_filename);
    std::ostream &ostream = (parms.output_filename.empty() or not ofile) ? std::cout : ofile;
    if (not ofile and not parms.output_filename.empty()) {
        std::cerr << "unable to open output file, falling back to standard output" << std::endl;
    }


    auto print_verbose = [& parms](std::string const &s) {
        if (parms.verbose) {
            std::cout << "[Verbose]: " << s << std::endl;
        }
    };


    STPFileParser parser(parms);
    bool instance_found = false;
    try {
        instance_found = parser.read_next_instance(istream);
        print_verbose("input parsed");
    } catch (std::invalid_argument const &e) {
        std::cerr << "Failed to read input. Error: " << e.what() << std::endl;
        return 1;
    }

    int instance_counter = 0;

    while (instance_found) {

        print_verbose("Starting Calculation of instance " + std::to_string(instance_counter));
        auto delaunay_graph = parser.create_delaunay_graph();

        delaunay_graph.calculate_l1_delaunay_triangulation();
        delaunay_graph.add_steiner_points();
        delaunay_graph.calculate_l1_delaunay_triangulation();
        print_verbose("Delaunay triangulation complete");

        MehlhornGraph mehlhorn_graph(delaunay_graph);
        mehlhorn_graph.calculate_mehlhorn_graph();
        print_verbose("Mehlhorn Graph calculated");
        mehlhorn_graph.kruskal_on_mehlhorn_edges();
        print_verbose("first kruskal complete");

        auto coordinate_graph = mehlhorn_graph.reconstruct_coord_graph_from_mehlhorn_edges();
        coordinate_graph.kruskal();
        print_verbose("second kruskal complete");

        coordinate_graph.l_shape_flipping();
        coordinate_graph.l_shape_flipping();
        print_verbose("l-shape-flipping complete");


        coordinate_graph.reduce_nodes();
        print_verbose("finished calculation of intance " + std::to_string(instance_counter++) + "\n");

        if (parms.output_format == OutputFormat::STP and parms.output) {
            coordinate_graph.print_as_stp(ostream);
        } else if (parms.output_format == OutputFormat::PS and parms.output) {
            coordinate_graph.print_as_postscript(ostream, "PostScript/template.ps");
        }
        ostream << std::endl << std::endl;

        try {
            instance_found = parser.read_next_instance(istream);
            if (instance_found) print_verbose("input parsed");
        } catch (std::invalid_argument const &e) {
            std::cerr << "Failed to read input. Error: " << e.what() << std::endl;
            return 1;
        }
    }

    if (ifile) ifile.close();
    if (ofile) ofile.close();

    return 0;
}
