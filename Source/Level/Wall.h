//
// Created by null on 9/29/2024.
//

#ifndef WALL_H
#define WALL_H
#include "../Math/Color.h"
#include "../Math/Vector.h"
#include "../Render/Mesh.h"
#include "../Render/Shader.h"
#include "../Render/Texture.h"
#include "../Math/WorldToScreen.h"
#include <glm/glm.hpp>

class Wall {
public:
    int id;
    Vector2 position{};
    Vector2 size;
    Color color{};
    Mesh mesh;
    Texture texture;
    bool isMeshSetupped = false;

    Wall(int id, Vector2 position, Vector2 size, Color color);
    void Render(Shader& shader);
    void GetScreenBounds(const glm::mat4& view, const glm::mat4& projection, float screenWidth, float screenHeight, Vector2& topLeft, Vector2& bottomRight) const;

    void ReapplyColor();
};
#endif //WALL_H
