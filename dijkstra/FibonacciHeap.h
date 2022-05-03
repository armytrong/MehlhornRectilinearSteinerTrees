//
// Created by jgier on 29.04.2022.
//

#ifndef HAUPTAUFGABE_2_FIBONACCIHEAP_H
#define HAUPTAUFGABE_2_FIBONACCIHEAP_H


#include <list>
#include <vector>

class FibonacciHeap {
private:
    struct Node {
        Node(int item_id, int key);

        Node(int item_id, int key, Node *parent);

        Node *parent;
        std::list<Node *> childs;
        bool is_binomial;
        int item_id;
        int key;
    };

    void plant(Node *node_ptr);

public:
    FibonacciHeap();

    void insert(int item_id, int key);

    int extract_min();

    [[maybe_unused]] int find_min() const;

    void decrease_key(int item_id, int new_key);

    bool empty() const;

    ~FibonacciHeap();

private:
    std::vector<Node *> _roots;
    std::vector<Node *> _created_nodes;
};


#endif //HAUPTAUFGABE_2_FIBONACCIHEAP_H
