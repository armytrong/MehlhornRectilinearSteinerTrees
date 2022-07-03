//
// Created by jgier on 29.04.2022.
//

#include <fstream>
#include <utility>
#include <sstream>
#include <cmath>
#include <cassert>
#include <iostream>
#include "STPFileParser.h"

constexpr char FILE_HEADER[] = "33D32945 STP File, STP Format Version 1.0";

STPFileParser::STPFileParser(ArgumentHandler::Parms const &parms) : _filename(std::move(parms.input_filename)) {

    _num_nodes = -1;

    std::ifstream file(_filename);
    if (not file and not _filename.empty()) {
        throw std::invalid_argument("input file not found");
    }
    std::istream &input = _filename.empty() ? std::cin : file;

    // skip the required number of instances in the input
    std::string line;
    safe_getline(input, line); // read the first line of the input
    for (size_t i = 0; i < parms.instances_to_skip + 1; i++) {
        while (line != FILE_HEADER and not input.eof()) {
            safe_getline(input, line);
            remove_carriage_return(line);
        }
        safe_getline(input, line);
    }

    if (input.eof()) {
        throw std::invalid_argument("Invalid input format: No stp-instance found");
    }

    bool graph_found = false;
    bool terminals_found = false;
    bool coordinates_found = false;

    while (line != "EOF") {
        if (not safe_getline(input, line)) {
            throw std::invalid_argument("Invalid input format: EOF expected");
        }
        if (line == "SECTION Graph" or line == "Section Graph") {
            if (graph_found) {
                throw std::invalid_argument("Invalid input format: Multiple graphs in input.");
            } else {
                read_graph_from_file(input);
                graph_found = true;
            }
        } else if (line == "SECTION Terminals" or line == "Section Terminals") {
            if (terminals_found) {
                throw std::invalid_argument("Invalid input format: Multiple terminal sections in input.");
            } else {
                read_terminals_from_file(input);
                terminals_found = true;
            }
        } else if (line == "SECTION Coordinates" or line == "Section Coordinates") {
            if (coordinates_found) {
                throw std::invalid_argument("Invalid input format: Multiple coordinate sections in input.");
            }
            if (parms.nodes_are_terminals) {
                _num_terminals = _num_nodes;
                _num_nodes = 10000 * 10000;
                read_coordinates_from_file(input, true);
            } else {
                read_coordinates_from_file(input, false);
            }
            coordinates_found = true;
        }
    }

    if (!graph_found) {
        throw std::invalid_argument("Invalid input format: No graph in input.");
    }

}

[[maybe_unused]] Graph STPFileParser::create_graph() {
    return {_num_nodes, _terminals, _edges};
}

void STPFileParser::read_graph_from_file(std::istream &file) {
    std::string line;
    std::string specifier;
    int val_1, val_2, val_3;
    int num_edges = 0;

    while (line != "END" and line != "End") {
        if (!safe_getline(file, line) or line == "EOF" or line == "EOF\r") {
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
            // TODO Hier sollte man auch gegen values die trotzdem 0 sind guarden
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

    while (line != "END" and line != "End") {
        if (!safe_getline(file, line) or line == "EOF" or line == "EOF\r") {
            throw std::invalid_argument("Invalid file format: END expected");
        }
        std::stringstream ss(line);
        ss >> specifier >> val;
        if (specifier == "Terminals") {
            _terminals.reserve(val);
            _num_terminals = val;
        } else if (specifier == "T") {
            // subtract 1 from node_id, this program is 0-based
            _terminals.push_back(val - 1);
        }
    }
    if ((size_t) _num_terminals != _terminals.size()) {
        throw std::invalid_argument("Invalid file format. Wrong number of terminals.");
    }
}

void STPFileParser::read_coordinates_from_file(std::istream &file, bool nodes_are_terminals) {
    std::string line;
    std::string specifier;
    int node_id, x_coord, y_coord;
    while (line != "END" and line != "End") {
        if (!safe_getline(file, line) or line == "EOF") {
            throw std::invalid_argument("Invalid file format: END expected");
        }
        std::stringstream ss(line);
        ss >> specifier >> node_id >> x_coord >> y_coord;

        if (not nodes_are_terminals) {
            _node_coords.resize(_num_nodes);
        }

        if (specifier == "DD") {
            // TODO where do the coordiantes begin, at 0,0 or 1,1?
            if (nodes_are_terminals) {
                _terminals.push_back((y_coord /* -1 */) * 10000 + x_coord /* -1 */ );
            } else {
                _node_coords[node_id] = {x_coord, y_coord};
            }
        }
    }
    if (nodes_are_terminals) {
        if ((size_t) _num_terminals != _terminals.size()) {
            throw std::invalid_argument("Invalid file format. Wrong number of terminals.");
        }
    }
}

inline void STPFileParser::remove_carriage_return(std::string &s) {
    while (s[s.size() - 1] == '\r') {
        s = s.erase(s.size() - 1);
    }
}

DelaunayGraph STPFileParser::create_delaunay_graph() {
    DelaunayGraph delaunay_graph;
    auto sqrt = static_cast<GridUnit>(std::sqrt(_num_nodes));
    if (_node_coords.empty()) {
        assert(sqrt * sqrt == _num_nodes);
    }
    for (auto terminal_id: _terminals) {
        if (_node_coords.empty()) {
            delaunay_graph.add_terminal(terminal_id % sqrt, static_cast<GridUnit>(terminal_id / sqrt));
        } else {
            delaunay_graph.add_terminal(_node_coords[terminal_id].x, _node_coords[terminal_id].y);
        }
    }
    return delaunay_graph;
}

inline std::istream &STPFileParser::safe_getline(std::istream &istream, std::string &string) {
    auto &ret = std::getline(istream, string);
    remove_carriage_return(string);
    return ret;
}


