//
// Created by jgier on 04.07.2022.
//

#ifndef HAUPTAUFGABE_2_GRAPHINTERFACE_H
#define HAUPTAUFGABE_2_GRAPHINTERFACE_H


#include "typedefs.h"

class GraphInterface {
    [[nodiscard]] virtual NodeId num_nodes() const = 0;
    [[nodiscard]] virtual NodeId num_edges() const = 0;
};


#endif //HAUPTAUFGABE_2_GRAPHINTERFACE_H
