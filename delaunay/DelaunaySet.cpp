//
// Created by jgier on 27.06.2022.
//

#include "DelaunaySet.h"


void DelaunaySet::insert(const DelaunaySet::Node &node) {
    _less_set.emplace(node);
    _greater_set.emplace(node);
}

void DelaunaySet::erase(const DelaunaySet::Node &node) {
    _less_set.erase(node);
    _greater_set.erase(node);
}

std::optional<DelaunaySet::Node> DelaunaySet::predecessor(const DelaunaySet::Node &node) const {
    auto ret = *_greater_set.upper_bound(node);
    return greater_fun(node, ret) ? std::make_optional(ret) : std::nullopt;
}

std::optional<DelaunaySet::Node> DelaunaySet::successor(const DelaunaySet::Node &node) const {
    auto ret = *_less_set.upper_bound(node);
    return less_fun(node, ret) ? std::make_optional(ret) : std::nullopt;
}
