#ifndef WORLDTOSCREEN_H
#define WORLDTOSCREEN_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include "Vector.h"

inline Vector2 worldToScreen(const Vector2& worldPos, const glm::mat4& view, const glm::mat4& projection, float screenWidth, float screenHeight) {
    glm::vec4 worldPos4 = glm::vec4(worldPos.x, worldPos.y, 0.0f, 1.0f);
    glm::vec4 clipSpace = projection * view * worldPos4;

    Vector2 screenPos;
    screenPos.x = (clipSpace.x / clipSpace.w + 1.0f) * 0.5f * screenWidth;
    screenPos.y = (1.0f - clipSpace.y / clipSpace.w) * 0.5f * screenHeight;  // Note: Y is flipped here

    return screenPos;
}

#endif
