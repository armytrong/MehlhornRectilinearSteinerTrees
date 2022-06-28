//
// Created by jgier on 24.06.2022.
//

#include "StandardHeap.h"

void StandardHeap::insert(int item_id, int key) {
    _heap.push_back({item_id, key});
    std::push_heap(_heap.begin(), _heap.end());
}

int StandardHeap::extract_min() {
    int min_id = _heap.front().item_id;
    std::pop_heap(_heap.begin(), _heap.end());
    _heap.pop_back();
    return min_id;
}

void StandardHeap::decrease_key(int item_id, int new_key) {
    for (int i = 0; i < _heap.size(); i++) {
        if (_heap[i].item_id == item_id) {
            _heap[i].key = new_key;
            std::push_heap(_heap.begin(), _heap.begin() + i + 1);
            break;
        }
    }
}

bool StandardHeap::empty() const {
    return _heap.empty();
}

StandardHeap::~StandardHeap() = default;

bool StandardHeap::Node::operator<(const StandardHeap::Node &other) const {
    return other.key < key;
}
