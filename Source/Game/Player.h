//
// Created by null on 9/28/2024.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <raylib.h>
#include <raymath.h>

constexpr int PLAYER_SPEED = 120;
constexpr Vector2 PLAYER_SIZE = Vector2(16.f, 16.f);

class Player {
public:
    int id;
    Vector2 position{};
    Color color{};

    Player(int id, Vector2 position, Color color);

    void Update(float dt, Vector2 move_dir);

    void Render() const;
};
#endif //PLAYER_H
