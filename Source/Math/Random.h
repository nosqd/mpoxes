//
// Created by null on 9/29/2024.
//

#ifndef RANDOM_H
#define RANDOM_H
#include "Vector.h"
#include <random>
inline std::random_device rd;
inline std::mt19937 gen(rd());

inline int randomInt(int min, int max) {
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

inline float randomFloat(float min, float max) {
    std::uniform_real_distribution<> distrib(min, max);
    return static_cast<float>(distrib(gen));
}

inline Vector2 randomVector2(float min, float max) {
    return Vector2(randomFloat(min, max), randomFloat(min, max));
}
#endif //RANDOM_H
