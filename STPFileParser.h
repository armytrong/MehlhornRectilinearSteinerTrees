//
// Created by jgier on 29.04.2022.
//

#ifndef HAUPTAUFGABE_2_STPFILEPARSER_H
#define HAUPTAUFGABE_2_STPFILEPARSER_H


#include <string>
#include "graph/Graph.h"

class STPFileParser {
public:
    explicit STPFileParser(std::string filename);

    Graph create_graph();

private:
    static bool check_file_header(std::ifstream &file);

    [[nodiscard]] std::ifstream open_input_file() const;

    void read_graph_from_file(std::istream &file);

    void read_terminals_from_file(std::istream &file);

    std::string _filename;
    Graph::NodeId _num_nodes;
    std::vector<Graph::NodeId> _terminals;
    std::vector<Graph::Edge> _edges;
};


#endif //HAUPTAUFGABE_2_STPFILEPARSER_H
