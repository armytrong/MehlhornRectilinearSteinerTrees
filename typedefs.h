//
// Created by jgier on 12.06.2022.
//

#ifndef HAUPTAUFGABE_2_TYPEDEFS_H
#define HAUPTAUFGABE_2_TYPEDEFS_H

#include <vector>
#include <iostream>

//#define DEBUG

using NodeId = int;
using EdgeId = int;
using GridUnit = int;
using WeightType = int;

constexpr NodeId INVALID_NODE = -1;
constexpr WeightType INVALID_WEIGHT = -1;


struct Coord {
    GridUnit x;
    GridUnit y;
};


#endif //HAUPTAUFGABE_2_TYPEDEFS_H
