//
// Created by null on 9/28/2024.
//

#include "Player.h"

#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>

#include "Mesh.h"
#include "Texture.h"

Player::Player(const int id, Vector2 position, Color color) {
    this->id = id;
    this->position = position;
    this->color = color;
}

void Player::Update(float dt, Vector2 move_dir) {
    this->position = this->position + move_dir * PLAYER_SPEED * dt;
}

void Player::Render(Shader shader) {
    if (mesh.VAO == -1) {
        std::vector<Vertex> quad_vertices = {
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f}}
        };
        std::vector<unsigned int> quad_indices = {
            0, 1, 2,
            2, 3, 0
        };
        Mesh quadMesh(quad_vertices, quad_indices);
        this->mesh = quadMesh;
        Texture texture(color.r, color.g, color.b, 255);
        this->texture = texture;
    }

    auto model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(PLAYER_SIZE.x, PLAYER_SIZE.y, 1.0f));
    model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));

    shader.setMat4("model", model);
    texture.use();
    mesh.Draw(shader);
}
