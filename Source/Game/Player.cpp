//
// Created by null on 9/28/2024.
//

#include "Player.h"

Player::Player(const int id, Vector2 position) {
    this->id = id;
    this->position = position;
}

void Player::Update(float dt, Vector2 move_dir) {
    this->position = Vector2Add(this->position, Vector2Scale(Vector2Scale(move_dir, PLAYER_SPEED), dt));
}

void Player::Render() const {
    DrawCircleV(Vector2Subtract(position, Vector2Divide(Vector2(PLAYER_SIZE, PLAYER_SIZE), Vector2(2.f, 2.f))), PLAYER_SIZE, RAYWHITE);
}
