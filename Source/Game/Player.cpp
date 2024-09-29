//
// Created by null on 9/28/2024.
//

#include "Player.h"

#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include "Game.h"
#include "../Math/Rect.h"
#include "../Render/Mesh.h"
#include "../Render/Texture.h"

Player::Player(const int id, Vector2 position, Color color, Game* game) {
    this->id = id;
    this->position = position;
    this->color = color;
    this->game = game;
#ifdef CLIENT
    std::vector<Vertex> quad_vertices = {
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}
    };

    std::vector<unsigned int> quad_indices = {
        0, 1, 2,
        2, 3, 0
    };
    Mesh quadMesh(quad_vertices, quad_indices);
    this->mesh = quadMesh;
    Texture texture(color.r, color.g, color.b, 255);
    this->texture = texture;
#endif
}

void Player::Update(float dt, Vector2 move_dir) {
    auto oldPosition = this->position;
    this->position = oldPosition + move_dir * PLAYER_SPEED * dt;

    Rect us = Rect{this->position, PLAYER_SIZE};
    for (auto wall : game->level->walls) {
        Rect rect = wall->GetRectangle();
        if (rect.Intersects(us)) {
            this->position = oldPosition;
        }
    }
}

void Player::Render(Shader& shader) {
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
    model = glm::scale(model, glm::vec3(PLAYER_SIZE.x, PLAYER_SIZE.y, 1.0f));

    shader.setMat4("model", model);
    texture.use();
    mesh.Draw(shader);
}
