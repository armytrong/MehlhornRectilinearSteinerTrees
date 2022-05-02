//
// Created by jgier on 29.04.2022.
//

#include <fstream>
#include <utility>
#include <sstream>
#include "STPFileParser.h"

constexpr char FILE_HEADER[] = "33D32945 STP File, STP Format Version 1.0";

STPFileParser::STPFileParser(std::string filename) : _filename(std::move(filename)) {
    std::ifstream file = open_input_file();

    _num_nodes = -1;

    std::string line;
    std::getline(file, line); // read the first line of the file
    while (line.empty())      // find the first non-empty line
    {
        std::getline(file, line);
    }
    // check for valid file header
    if (line != FILE_HEADER) {
        throw std::invalid_argument("Invalid file format.");
    }

    bool graph_found = false;
    bool terminals_found = false;


    while (line != "EOF") {
        std::getline(file, line);
        if (line == "SECTION Graph") {
            if (graph_found) {
                throw std::invalid_argument("Invalid file format: Multiple graphs in file.");
            } else {
                read_graph_from_file(file);
                graph_found = true;
            }
        } else if (line == "SECTION Terminals") {
            if (terminals_found) {
                throw std::invalid_argument("Invalid file format: Multiple terminal lists in file.");
            } else {
                read_terminals_from_file(file);
                terminals_found = true;
            }
        }
    }

    if (!graph_found) {
        throw std::invalid_argument("Invalid file format: No graph in file.");
    }

}

Graph STPFileParser::create_graph() {
    return Graph(_num_nodes, _terminals, _edges);
}

bool STPFileParser::check_file_header(std::ifstream &file) {
    std::string line;
    std::getline(file, line); // read the first line of the file
    // FIXME: wie kann ich hier eine Endlosschleife sicher vermeiden?
    while (line.empty() or !file.eof())      // find the first non-empty line
    {
        std::getline(file, line);
    }
    // check for valid file header
    return !(line == FILE_HEADER);
}

void STPFileParser::read_graph_from_file(std::istream &file) {
    std::string line;
    std::string specifier;
    int val_1, val_2, val_3;
    int num_edges;

    while (line != "END") {
        if (!std::getline(file, line) or line == "EOF") {
            throw std::invalid_argument("Invalid file format.");
        }
        std::stringstream ss(line);
        ss >> specifier >> val_1 >> val_2 >> val_3;
        if (specifier == "Nodes") {
            _num_nodes = val_1;
        } else if (specifier == "Edges") {
            _edges.reserve(val_1);
            num_edges = val_1;
        } else if (specifier == "E") {
            if (val_1 < 1 or val_1 > _num_nodes or val_2 < 1 or val_2 > _num_nodes) {
                throw std::invalid_argument("Invalid file foramt: Invalid edge.");
            }
            // FIXME Hier sollte man auch gegen values die trotzdem 0 sind guarden
            // subtract 1 from node_id, this program is 0-based
            _edges.emplace_back(val_1 - 1, val_2 - 1, val_3);
        }
    }

    if ((size_t) num_edges != _edges.size()) {
        throw std::invalid_argument("Invalid file format: Wrong number of edges.");
    }
}


void STPFileParser::read_terminals_from_file(std::istream &file) {
    std::string line;
    std::string specifier;
    int val;
    int num_terminals;

    while (line != "END") {
        if (!std::getline(file, line) or line == "EOF") {
            throw std::invalid_argument("Invalid file format.");
        }
        std::stringstream ss(line);
        ss >> specifier >> val;
        if (specifier == "Terminals") {
            _terminals.reserve(val);
            num_terminals = val;
        } else if (specifier == "T") {
            // subtract 1 from node_id, this program is 0-based
            _terminals.push_back(val - 1);
        }
    }
    if ((size_t) num_terminals != _terminals.size()) {
        throw std::invalid_argument("Invalid file format. Wrong number of terminals.");
    }
}

std::ifstream STPFileParser::open_input_file() const {
    std::ifstream file{_filename, std::ifstream::in};
    // open file
    if (!file) {
        throw std::invalid_argument("Cannot open file " + _filename + ".");
    }
    //check_file_header(file);
    return file;
}
