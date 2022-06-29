//
// Created by jgier on 29.06.2022.
//

#include "ArgumentHandler.h"
#include <fstream>

const ArgumentHandler::Parms &ArgumentHandler::parms() {
    return _parms;
}

ArgumentHandler::ArgumentHandler(int argc, char **argv) : _parms{"", "", OutputFormat::STP, 0, false, true} {
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
            print_usage(argv[0]);
            _parms.run = false;
        } else if (_parms.input_filename.empty()) {
            _parms.input_filename = arg;
        }
    }
}

void ArgumentHandler::print_usage(std::string const &program_name) {
    std::cerr << "Usage: " << program_name
              << " [-ps] [-stp] [-o OUTPUT_FILE] [-mi] [-mo] [-v] [INPUT_FILENAME]"
              << std::endl;
}
