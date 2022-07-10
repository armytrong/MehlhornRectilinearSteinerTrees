//
// Created by jgier on 29.04.2022.
//

#ifndef HAUPTAUFGABE_2_FIBONACCIHEAP_H
#define HAUPTAUFGABE_2_FIBONACCIHEAP_H


#include <list>
#include <vector>
#include "PriorityQueue.h"

class [[maybe_unused]] FibonacciHeap : public PriorityQueue {
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

    [[maybe_unused]] void insert(int item_id, int key) override;

    [[maybe_unused]] int extract_min() override;

    [[maybe_unused]] [[nodiscard]] int find_min() const;

    [[maybe_unused]] void decrease_key(int item_id, int new_key) override;

    [[maybe_unused]] [[nodiscard]] bool empty() const override;

    ~FibonacciHeap() override;

private:
    std::vector<Node *> _roots;
    std::vector<Node *> _created_nodes;
};


#endif //HAUPTAUFGABE_2_FIBONACCIHEAP_H
