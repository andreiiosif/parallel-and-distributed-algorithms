//
// Created by Andrei Ioja on 09.11.2023.
//

#ifndef LAB_6_RANDOMNAME_H
#define LAB_6_RANDOMNAME_H

#include <vector>
#include <string>

struct Distribution {
    float mean, deviation;
};

class RandGenerator {
public:
    static float generate_real(float mean, float deviation);
};


#endif //LAB_6_RANDOMNAME_H
