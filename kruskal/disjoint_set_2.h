#pragma once

#include <iostream>
#include <vector>

class Disjoint_Set {
public:
    class Node {
    public:
        Node();

        void set_parent(Node *parent);

        void increment_depth();

        [[nodiscard]] Node *get_parent() const;

        [[nodiscard]] int get_depth() const;

    private:
        Node *_parent;
        int _depth;
    };

    bool set_equals(int a, int b);

    void make_sets(int num_new_sets);

    Node *find(Node *node);

    void unite(int a, int b);

    [[maybe_unused]] [[nodiscard]] int get_num_nodes() const;

private:
    std::vector<Node> _nodes;
};

//Hier waere dann eine inline section

//z.B. 
// inline bool Disjoint_Set::set_equals(int a, int b) {
//    code
// }

inline Disjoint_Set::Node::Node() {
    _parent = this;
    _depth = 0;
}

// getter- and setter-methods
inline void Disjoint_Set::Node::set_parent(Node *parent) {
    _parent = parent;
}

inline void Disjoint_Set::Node::increment_depth() {
    _depth++;
}

inline Disjoint_Set::Node *Disjoint_Set::Node::get_parent() const {
    return _parent;
}

inline int Disjoint_Set::Node::get_depth() const {
    return _depth;
}

// end of class Node //

// the functionality of -make_set- from the lecture has been expanded to
//create an arbitrary amount of sets at once.
inline void Disjoint_Set::make_sets(int num_new_sets) {
    _nodes.resize(_nodes.size() + num_new_sets);
}

// the -find- method from the lecture
inline Disjoint_Set::Node *Disjoint_Set::find(Node *node) {
    Node *parent = node->get_parent();

    if (parent != node) {
        Node *new_parent = find(parent->get_parent());
        node->set_parent(new_parent);
    }

    return node->get_parent();
}

// -union- method from the lecture (union is a keyword in c++, thus it's called unite)
inline void Disjoint_Set::unite(int a, int b) {
    Node *a_ptr = &_nodes[a];
    Node *b_ptr = &_nodes[b];

    Node *parent_a = find(a_ptr);
    Node *parent_b = find(b_ptr);

    if (parent_a->get_depth() > parent_b->get_depth()) {
        parent_b->set_parent(parent_a);
    } else {
        parent_a->set_parent(parent_b);
        if (parent_a->get_depth() == parent_b->get_depth()) {
            parent_b->increment_depth();
        }
    }
}

inline bool Disjoint_Set::set_equals(int a, int b) {
    if (find(&_nodes[a]) == find(&_nodes[b]))
        return true;
    return false;
}

inline int Disjoint_Set::get_num_nodes() const {
    return static_cast<int>(_nodes.size());
}

