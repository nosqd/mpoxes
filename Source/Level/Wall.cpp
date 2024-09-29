//
// Created by null on 9/29/2024.
//

#include "Wall.h"

#include <glm/ext/matrix_transform.hpp>

Wall::Wall(int id, Vector2 position, Vector2 size, Color color) {
    this->id = id;
    this->position = position;
    this->size = size;
    this->color = color;
    this->mesh = Mesh();
    this->texture = Texture();
}

void Wall::Render(Shader& shader) {
    if (!this->isMeshSetupped) {
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
        spdlog::info("Wall {} recreating mesh", id);
        ReapplyColor();
        this->isMeshSetupped = true;
    }

    auto model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
    model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f));

    shader.setMat4("model", model);
    texture.use();
    mesh.Draw(shader);
}

void Wall::GetScreenBounds(const glm::mat4& view, const glm::mat4& projection, float screenWidth, float screenHeight, Vector2& topLeft, Vector2& bottomRight) const {
    // Calculate world space coordinates of the wall's corners
    Vector2 worldTopLeft(position.x, position.y + size.y);
    Vector2 worldBottomRight(position.x + size.x, position.y);

    // Convert to screen space
    topLeft = worldToScreen(worldTopLeft, view, projection, screenWidth, screenHeight);
    bottomRight = worldToScreen(worldBottomRight, view, projection, screenWidth, screenHeight);
}

void Wall::ReapplyColor() {
    spdlog::info("Wall {} recreating texture", id);
    Texture texture(color.r, color.g, color.b, 255);
    this->texture = texture;
}
