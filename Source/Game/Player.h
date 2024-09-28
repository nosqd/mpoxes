//
// Created by null on 9/28/2024.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <raylib.h>
#include <raymath.h>

constexpr int PLAYER_SPEED = 120;
//constexpr Vector2 PLAYER_SIZE = Vector2{32.0f, 32.0f};
constexpr int PLAYER_SIZE = 8.f;

class Player {
private:

    Vector2 last_drawn_position{};

public:
    int id;
    Vector2 position{};

    Player(const int id, Vector2 position);

    void Update(float dt, Vector2 move_dir);
    void Render() const;
};
#endif //PLAYER_H
