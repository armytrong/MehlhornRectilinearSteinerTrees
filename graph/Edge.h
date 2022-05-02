//
// Created by jgier on 29.04.2022.
//

#ifndef HAUPTAUFGABE_2_EDGE_H
#define HAUPTAUFGABE_2_EDGE_H


class Edge
{
public:
    //trivial constructor, nothing interesting
    Edge(int node_a, int node_b, int weight = 1)
    {
        _nodes[0] = node_a;
        _nodes[1] = node_b;
        _weight = weight;
    }

    // < is needed for sorting the edges.
    bool operator<(Edge other)
    {
        if (_weight < other._weight)
            return true;
        return false;
    }

    //Edges consist of their nodes and their weight.
    int _nodes[2];
    int _weight;
};


#endif //HAUPTAUFGABE_2_EDGE_H
