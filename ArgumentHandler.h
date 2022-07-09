//
// Created by jgier on 29.06.2022.
//

#ifndef HAUPTAUFGABE_2_ARGUMENTHANDLER_H
#define HAUPTAUFGABE_2_ARGUMENTHANDLER_H


#include <optional>
#include <string>
#include <iostream>
#include <fstream>

enum struct OutputFormat {
    PS, STP
};

class ArgumentHandler {
public:
    struct Parms {
        std::string input_filename;
        std::string output_filename;
        OutputFormat output_format;
        size_t instances_to_skip;
        bool nodes_are_terminals;
        bool verbose;
        bool output;
        bool run;
    };

    ArgumentHandler(int argc, char **argv);

    static void print_usage(std::ostream &os, std::string const &program_name);

    Parms const &parms();

private:
    Parms _parms;
    std::optional<std::ofstream> _file;
};


#endif //HAUPTAUFGABE_2_ARGUMENTHANDLER_H
