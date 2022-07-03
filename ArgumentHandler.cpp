//
// Created by jgier on 29.06.2022.
//

#include "ArgumentHandler.h"
#include <fstream>

const ArgumentHandler::Parms &ArgumentHandler::parms() {
    return _parms;
}

ArgumentHandler::ArgumentHandler(int argc, char **argv) : _parms{
        "",
        "",
        OutputFormat::STP,
        0,
        false,
        false,
        true
} {
    for (int i = 1; i < argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-ps") {
            _parms.output_format = OutputFormat::PS;
        } else if (arg == "-stp") {
            _parms.output_format = OutputFormat::STP;
        } else if (arg == "-o") {
            i++;
            if (i < argc) {
                _parms.output_filename = argv[i];
            }
        } else if (arg == "-v" or arg == "--verbose") {
            _parms.verbose = true;
        } else if (arg == "-mi" or arg == "--manual_input") {
            std::cout << "Specify input file: ";
            std::cin >> _parms.input_filename;
        } else if (arg == "-mo" or arg == "--manual_output") {
            std::cout << "Specify output file: ";
            std::cin >> _parms.output_filename;
        } else if (arg == "-h" or arg == "--help") {
            print_usage(std::cout, argv[0]);
            _parms.run = false;
        } else if (arg == "-t" or arg == "--nodes-are-terminals") {
            _parms.nodes_are_terminals = true;
        } else if (arg == "-s" or arg == "--skip-instances") {
            i++;
            if (i < argc) {
                _parms.instances_to_skip = std::stoi(argv[i]);
            }
        } else if (arg[0] == '-') {
            std::cerr << argv[0] << ": " << arg << ": invalid option" << std::endl;
            print_usage(std::cerr, argv[0]);
            _parms.run = false;
        } else if (_parms.input_filename.empty()) {
            _parms.input_filename = arg;
        }
    }
}

void ArgumentHandler::print_usage(std::ostream &os, std::string const &program_name) {
    os << "Usage: " << program_name
       << " [-ps | -stp] [-o OUTPUT_FILE] [-mi] [-mo] [-v] [-t] [-s NUM] [INPUT_FILENAME]"
       << std::endl << std::endl;
    os << "Options: " << std::endl;
    os << "\t -ps | -stp \t\t output result as PostScript or .stp respectively" << std::endl;
    os << "\t -o OUTPUT_FILE \t specify file to write output to. Blank for standard output" << std::endl;
    os << "\t -mi -mo \t\t Specify input respectively output file during runtime via standard input" << std::endl;
    os << "\t -v \t\t\t print verbose information" << std::endl;
    os << "\t -t \t\t\t treat nodes as terminals" << std::endl;
    os << "\t -s NUMBER \t\t skip number of instances in input" << std::endl;
}
