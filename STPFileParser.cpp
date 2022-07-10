//
// Created by jgier on 29.04.2022.
//

#include <sstream>
#include <cmath>
#include <cassert>
#include <iostream>
#include "STPFileParser.h"


STPFileParser::STPFileParser(ArgumentHandler::Parms const &parms) :
        _parms(parms), _num_nodes(INVALID_NODE), _num_terminals(INVALID_NODE) {}

void STPFileParser::read_graph_from_file(std::istream &file) {
    std::string line;
    std::string specifier;
    int val_1, val_2, val_3;

    while (line != "END" and line != "End") {
        if (!safe_getline(file, line) or line == "EOF" or line == "EOF\r") {
            throw std::invalid_argument("Invalid file format.");
        }
        std::stringstream ss(line);
        ss >> specifier >> val_1 >> val_2 >> val_3;
        if (specifier == "Nodes") {
            _num_nodes = val_1;
        } else if (specifier == "Edges") {
        } else if (specifier == "E") {
        }
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
            // subtract 1 from original_node_id, this program is 0-based
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
            if (nodes_are_terminals) {
                _terminals.push_back((y_coord /* -1 */) * 10000 + x_coord /* -1 */ );
            } else {
                _node_coords[node_id - 1] = {x_coord, y_coord};
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
    while (not s.empty() and s[s.size() - 1] == '\r') {
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

bool STPFileParser::read_next_instance(std::istream &istream) {
    _num_nodes = INVALID_NODE;
    _num_terminals = INVALID_NODE;
    _terminals.clear();
    _node_coords.clear();

    // skip the required number of instances in the input
    std::string line;
    safe_getline(istream, line); // read the first line of the input
    for (size_t i = 0; i < _parms.instances_to_skip + 1; i++) {
        while (line != FILE_HEADER and not istream.eof()) {
            safe_getline(istream, line);
            remove_carriage_return(line);
        }
        safe_getline(istream, line);
    }

    if (istream.eof()) {
        return false;
    }

    bool graph_found = false;
    bool terminals_found = false;
    bool coordinates_found = false;

    while (line != "EOF") {
        if (not safe_getline(istream, line)) {
            throw std::invalid_argument("Invalid input format: EOF expected");
        }
        if (line == "SECTION Graph" or line == "Section Graph") {
            if (graph_found) {
                throw std::invalid_argument("Invalid input format: Multiple graphs in input.");
            } else {
                read_graph_from_file(istream);
                graph_found = true;
            }
        } else if (line == "SECTION Terminals" or line == "Section Terminals") {
            if (terminals_found) {
                throw std::invalid_argument("Invalid input format: Multiple terminal sections in input.");
            } else {
                read_terminals_from_file(istream);
                terminals_found = true;
            }
        } else if (line == "SECTION Coordinates" or line == "Section Coordinates") {
            if (coordinates_found) {
                throw std::invalid_argument("Invalid input format: Multiple coordinate sections in input.");
            }
            if (_parms.nodes_are_terminals) {
                _num_terminals = _num_nodes;
                _num_nodes = 10000 * 10000;
                read_coordinates_from_file(istream, true);
            } else {
                read_coordinates_from_file(istream, false);
            }
            coordinates_found = true;
        }
    }

    if (!graph_found) {
        throw std::invalid_argument("Invalid input format: No graph in input.");
    }
    return true;
}



