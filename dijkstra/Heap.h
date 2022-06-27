//
// Created by jgier on 27.06.2022.
//

#ifndef HAUPTAUFGABE_2_HEAP_H
#define HAUPTAUFGABE_2_HEAP_H


class Heap {
public:
    using IdType = int;
    using KeyType = int;

    virtual void insert(IdType item_id, KeyType key) = 0;

    virtual int extract_min() = 0;

    virtual void decrease_key(int item_id, int new_key) = 0;

    [[nodiscard]] virtual bool empty() const = 0;

    virtual ~Heap() = 0;
};


#endif //HAUPTAUFGABE_2_HEAP_H
