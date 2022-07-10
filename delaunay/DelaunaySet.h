//
// Created by jgier on 27.06.2022.
//

#ifndef HAUPTAUFGABE_2_DELAUNAYSET_H
#define HAUPTAUFGABE_2_DELAUNAYSET_H

#include "graph/CoordinateGraph.h"
#include <set>

class DelaunaySet {
public:
    using Node = CoordinateGraph::Node;

    void insert(Node const &node);
    void erase(Node const &node);
    [[nodiscard]] std::optional<DelaunaySet::Node> successor(Node const &node) const;
    [[nodiscard]] std::optional<DelaunaySet::Node> predecessor(Node const &node) const;

private:
    static constexpr auto less_fun = [](Node const &lhs, Node const &rhs) {
        return (lhs.y_coord < rhs.y_coord) or (rhs.y_coord == lhs.y_coord and rhs.x_coord < lhs.x_coord);
    };
    static constexpr auto greater_fun = [](Node const &lhs, Node const &rhs) {
        return (lhs.y_coord > rhs.y_coord) or (rhs.y_coord == lhs.y_coord and rhs.x_coord > lhs.x_coord);
    };

    std::set<Node, decltype(less_fun)> _less_set;
    std::set<Node, decltype(greater_fun)> _greater_set;
};


#endif //HAUPTAUFGABE_2_DELAUNAYSET_H
