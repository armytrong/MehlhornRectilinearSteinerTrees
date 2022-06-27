//
// Created by jgier on 27.06.2022.
//

#ifndef HAUPTAUFGABE_2_BINARYHEAP_H
#define HAUPTAUFGABE_2_BINARYHEAP_H


#include "Heap.h"

class BinaryHeap : public Heap {
public:
    void insert(IdType item_id, KeyType key) override;
    int extract_min() override;
    void decrease_key(int item_id, int new_key) override;
    bool empty() const override;
    ~BinaryHeap() override;
};


#endif //HAUPTAUFGABE_2_BINARYHEAP_H
