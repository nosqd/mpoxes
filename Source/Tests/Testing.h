//
// Created by null on 9/29/2024.
//

#ifndef TESTING_H
#define TESTING_H

inline void compareLevels(Level *expectedLevel, Level *level) {
    REQUIRE(level->walls.size() == expectedLevel->walls.size());
    for (size_t i = 0; i < level->walls.size(); ++i) {
        auto expected = expectedLevel->walls[i];
        auto got = level->walls[i];

        REQUIRE(expected->id == got->id);
        REQUIRE(expected->position == got->position);
        REQUIRE(expected->size == got->size);
        REQUIRE(expected->color == got->color);
    }
}

#endif //TESTING_H
