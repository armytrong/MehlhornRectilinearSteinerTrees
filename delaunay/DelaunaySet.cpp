//
// Created by jgier on 27.06.2022.
//

#include <cassert>
#include "DelaunaySet.h"


void DelaunaySet::insert(const DelaunaySet::Terminal &t) {
    _less_set.emplace(t);
    _greater_set.emplace(t);
}

void DelaunaySet::del(const DelaunaySet::Terminal &t) {
    _less_set.erase(t);
    _greater_set.erase(t);
}

std::optional<DelaunaySet::Terminal> DelaunaySet::predecessor(const DelaunaySet::Terminal &t) const {
    auto ret = *_greater_set.upper_bound(t);
    //assert(greater_fun(t, ret));
    return greater_fun(t, ret) ? std::make_optional(ret) : std::nullopt;
}

std::optional<DelaunaySet::Terminal> DelaunaySet::sucessor(const DelaunaySet::Terminal &t) const {
    auto ret = *_less_set.upper_bound(t);
    //assert(less_fun(t, ret));
    return less_fun(t, ret) ? std::make_optional(ret) : std::nullopt;
}
