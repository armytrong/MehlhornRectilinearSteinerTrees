//
// Created by jgier on 27.06.2022.
//

#ifndef HAUPTAUFGABE_2_DELAUNAYSET_H
#define HAUPTAUFGABE_2_DELAUNAYSET_H

#include "DelaunayGraph.h"
#include <set>

class DelaunaySet {
public:
    using Terminal = DelaunayGraph::Node;

    void insert(Terminal const &t);
    void del(Terminal const &t);
    [[nodiscard]] std::optional<DelaunaySet::Terminal> successor(Terminal const &t) const;
    [[nodiscard]] std::optional<DelaunaySet::Terminal> predecessor(Terminal const &t) const;

private:
    static constexpr auto less_fun = [](Terminal const &lhs, Terminal const &rhs) {
        return (lhs.y_coord < rhs.y_coord) or (rhs.y_coord == lhs.y_coord and rhs.x_coord < lhs.x_coord);
    };
    static constexpr auto greater_fun = [](Terminal const &lhs, Terminal const &rhs) {
        return (lhs.y_coord > rhs.y_coord) or (rhs.y_coord == lhs.y_coord and rhs.x_coord > lhs.x_coord);
    };

    std::set<Terminal, decltype(less_fun)> _less_set;
    std::set<Terminal, decltype(greater_fun)> _greater_set;
};


#endif //HAUPTAUFGABE_2_DELAUNAYSET_H
