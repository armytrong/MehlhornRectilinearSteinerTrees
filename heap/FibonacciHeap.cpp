//
// Created by jgier on 29.04.2022.
//

#include <stack>
#include <stdexcept>
#include <cassert>
#include "FibonacciHeap.h"

FibonacciHeap::Node::Node(int item_id, int key) {
    parent = nullptr;
    is_binomial = true;
    this->item_id = item_id;
    this->key = key;
}

FibonacciHeap::Node::Node(int item_id, int key, Node *parent) {
    this->parent = parent;
    is_binomial = true;
    this->item_id = item_id;
    this->key = key;
}

FibonacciHeap::FibonacciHeap() = default;

void FibonacciHeap::insert(int item_id, int key) {

    if (_created_nodes.size() <= (size_t) item_id) {
        _created_nodes.resize(item_id + 1, nullptr);
    }
    if (_created_nodes[item_id] == nullptr) {
        _created_nodes[item_id] = new Node(item_id, key);
        plant(_created_nodes[item_id]);
    } else {
        throw std::invalid_argument("Item already in Heap.");
    }
}

void FibonacciHeap::plant(Node *v) { // NOLINT(misc-no-recursion)
    v->is_binomial = true;

    if (_roots.size() < v->childs.size() + 1) {
        _roots.resize(v->childs.size() + 1, nullptr);
    }

    Node *r = _roots[v->childs.size()];
    if (r == nullptr || r == v || r->parent != nullptr) {
        _roots[v->childs.size()] = v;
        v->parent = nullptr;
    } else {
        _roots[v->childs.size()] = nullptr;
        if (r->key <= v->key) {
            r->childs.push_back(v);
            v->parent = r;
            plant(r);
        } else {
            v->childs.push_back(r);
            r->parent = v;
            plant(v);
        }
    }
}

int FibonacciHeap::extract_min() {
    int min_index = 0;
    while (_roots[min_index] == nullptr) {
        min_index++;
        if ((size_t) min_index == _roots.size()) {
            throw std::out_of_range("Heap is empty.");
        }
    }
    for (int i = 0; i < _roots.size(); i++) {
        if (_roots[i] != nullptr) {

            // These asserts are only sensible, if I find a notion of knowing the maximum possible index,
            // istead of choosing an arbitrary (large) value i.e. 10000.

            // assert(_roots[i]->key >= 0 && _roots[i]->key <= 10000);
            // assert(_roots[min_index]->key >= 0 && _roots[i]->key <= 10000);

            if (_roots[i]->key < _roots[min_index]->key) {
                min_index = i;
            }
        }
    }

    Node *min_node = _roots[min_index];
    _roots[min_index] = nullptr;
    for (Node *child: min_node->childs) {
        plant(child);
    }
    return min_node->item_id;
}

int FibonacciHeap::find_min() const {
    int min_index = 0;
    for (int i = 0; i < _roots.size(); i++) {
        if (_roots[i] != nullptr && _roots[i]->key < _roots[min_index]->key) {
            min_index = i;
        }
    }
    Node *ret = _roots[min_index];
    return ret->key;
}

void FibonacciHeap::decrease_key(int item_id, int new_key) {
    Node *node_ptr = _created_nodes[item_id];
    node_ptr->key = new_key;
    node_ptr->is_binomial = false;
    std::stack<Node *> nodes_to_plant;
    Node *child = node_ptr;
    Node *parent = node_ptr->parent;

    while (!child->is_binomial && parent != nullptr) {
        parent->childs.remove(child);
        child->parent = nullptr;
        nodes_to_plant.push(child);
        child = parent;
        parent = child->parent;
    }
    if (parent != nullptr) {
        parent->is_binomial = false;
    }

    while (!nodes_to_plant.empty()) {
        plant(nodes_to_plant.top());
        nodes_to_plant.pop();
    }
}

bool FibonacciHeap::empty() const {
    if (std::all_of(_roots.begin(), _roots.end(), [](Node *n) { return n == nullptr; })) {
        return true;
    }
    return false;
}

FibonacciHeap::~FibonacciHeap() {
    while (!_created_nodes.empty()) {
        delete _created_nodes.back();
        _created_nodes.pop_back();
    }
}