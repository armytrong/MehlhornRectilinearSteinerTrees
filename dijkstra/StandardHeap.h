//
// Created by jgier on 24.06.2022.
//

#ifndef HAUPTAUFGABE_2_STANDARDHEAP_H
#define HAUPTAUFGABE_2_STANDARDHEAP_H


#include <vector>

class StandardHeap {
private:
    struct Node {
        int item_id, key;
        bool operator<(Node const &other) const;
    };

public:
    void insert(int item_id, int key);
    int extract_min();
    void decrease_key(int item_id, int new_key);
    [[nodiscard]] bool empty() const;
private:
    std::vector<Node> _heap;
};


#endif //HAUPTAUFGABE_2_STANDARDHEAP_H
