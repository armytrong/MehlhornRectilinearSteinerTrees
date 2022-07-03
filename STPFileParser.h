//
// Created by jgier on 29.04.2022.
//

#ifndef HAUPTAUFGABE_2_STPFILEPARSER_H
#define HAUPTAUFGABE_2_STPFILEPARSER_H


#include <string>
#include "graph/Graph.h"
#include "delaunay/DelaunayGraph.h"
#include "ArgumentHandler.h"


class STPFileParser {
public:
    explicit STPFileParser(ArgumentHandler::Parms const &parms);

    [[maybe_unused]] Graph create_graph();

    DelaunayGraph create_delaunay_graph();

private:

    void read_graph_from_file(std::istream &file);

    void read_terminals_from_file(std::istream &file);

    void read_coordinates_from_file(std::istream &file, bool nodes_are_terminals);

    static void remove_carriage_return(std::string &s);

    static std::istream &safe_getline(std::istream &istream, std::string &string);

    std::string _filename;
    NodeId _num_nodes;
    NodeId _num_terminals;
    std::vector<NodeId> _terminals;
    std::vector<Graph::Edge> _edges;
    std::vector<Coord> _node_coords;
};


#endif //HAUPTAUFGABE_2_STPFILEPARSER_H
