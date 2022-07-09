//
// Created by jgier on 28.06.2022.
//

#include <cassert>
#include "DelaunayPriorityQueue.h"


DelaunayPriorityQueue::Record DelaunayPriorityQueue::extract_min() {
    auto ret = *_set.lower_bound(MINIMUM_RECORD);
    assert(_set.contains(ret));
    _set.erase(ret);
    return ret;
}

void DelaunayPriorityQueue::insert(const DelaunayPriorityQueue::Terminal &t, GridUnit trans, ActiveInactive status) {
    //assert(t.x_coord >= 0 and t.y_coord >= 0 and trans >= 0);
    _set.insert({t, trans, status});
}

std::optional<DelaunayPriorityQueue::Record> DelaunayPriorityQueue::find_inactivation_record
        (DelaunayPriorityQueue::Terminal t) const {
    auto record = *_set.lower_bound({t, 0, ActiveInactive::INACTIVE});
    if (record.terminal == t and record.status == ActiveInactive::INACTIVE) {
        return std::make_optional(record);
    } else {
        return std::nullopt;
    }
}

void DelaunayPriorityQueue::change_priority(const DelaunayPriorityQueue::Terminal &t, GridUnit new_trans) {
    auto record = *_set.lower_bound({t, 0, ActiveInactive::INACTIVE});
    assert(record.status == ActiveInactive::INACTIVE);
    _set.erase(record);
    _set.insert({t, new_trans, ActiveInactive::INACTIVE});
}

bool DelaunayPriorityQueue::empty() const {
    return _set.empty();
}
