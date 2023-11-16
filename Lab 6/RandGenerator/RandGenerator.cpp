//
// Created by Andrei Ioja on 09.11.2023.
//

#include "RandGenerator.h"
#include <random>


float RandGenerator::generate_real(float mean, float deviation) {
// create generator for random
    std::random_device rd{};
    std::mt19937 gen{rd()};

    // generate a value using normal distribution
    std::normal_distribution d{mean, deviation};
    auto random_float = [&d, &gen] {return d(gen); };

    return random_float();
}
