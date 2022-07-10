//
// Created by jgier on 24.06.2022.
//

#ifndef HAUPTAUFGABE_2_STANDARDHEAP_H
#define HAUPTAUFGABE_2_STANDARDHEAP_H


#include <vector>
#include "PriorityQueue.h"

class StandardHeap : public PriorityQueue {
private:
    struct Node {
        int item_id, key;
        bool operator<(Node const &other) const;
    };

public:
    ~StandardHeap() override;
public:
    void insert(int item_id, int key) override;
    int extract_min() override;
    void decrease_key(int item_id, int new_key) override;
    [[nodiscard]] bool empty() const override;
private:
    std::vector<Node> _heap;
};


#endif //HAUPTAUFGABE_2_STANDARDHEAP_H
