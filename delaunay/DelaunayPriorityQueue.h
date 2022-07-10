//
// Created by jgier on 28.06.2022.
//

#ifndef HAUPTAUFGABE_2_DELAUNAYPRIORITYQUEUE_H
#define HAUPTAUFGABE_2_DELAUNAYPRIORITYQUEUE_H


#include <set>
#include "graph/CoordinateGraph.h"

enum class ActiveInactive {
    INACTIVE, ACTIVE
};


class DelaunayPriorityQueue {
public:
    DelaunayPriorityQueue(GridUnit max_x, GridUnit max_y) : MINIMUM_RECORD(
            {{-max_x - 1, -max_y - 1}, -max_x - max_y - 1, ActiveInactive::INACTIVE}) {}

    using Terminal = CoordinateGraph::Node;
    struct Record {
        Terminal terminal;
        GridUnit trans;
        ActiveInactive status;
    };

    Record extract_min();
    void insert(Terminal const &t, GridUnit trans, ActiveInactive status);
    void change_priority(Terminal const &t, GridUnit new_trans);
    [[nodiscard]] std::optional<Record> find_inactivation_record(Terminal t) const;
    [[nodiscard]] bool empty() const;

private:
    static constexpr auto greater_fun = [](Record const &rhs, Record const &lhs) {
        return (lhs.trans > rhs.trans) or (lhs.trans == rhs.trans and lhs.terminal.y_coord > rhs.terminal.y_coord)
               or (lhs.trans == rhs.trans and lhs.terminal.y_coord == rhs.terminal.y_coord and lhs.status < rhs.status);
    };
    std::set<Record, decltype(greater_fun)> _set;

    const DelaunayPriorityQueue::Record MINIMUM_RECORD;
};


#endif //HAUPTAUFGABE_2_DELAUNAYPRIORITYQUEUE_H
