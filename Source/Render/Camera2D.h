//
// Created by null on 9/29/2024.
//

#ifndef CAMERA2D_H
#define CAMERA2D_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera2D {
public:
    Camera2D(float screenWidth, float screenHeight)
        : position(0.f, 0.f), zoom(1.0f), screenWidth(screenWidth), screenHeight(screenHeight) {}

    void move(float dx, float dy) {
        position.x += dx;
        position.y += dy;
    }

    void setPosition(float x, float y) {
        position.x = x;
        position.y = y;
    }

    void setZoom(float newZoom) {
        zoom = newZoom;
    }

    glm::mat4 getViewMatrix() const {
        return glm::translate(glm::mat4(1.0f), glm::vec3(-position.x, -position.y, 0.0f));
    }

    glm::mat4 getProjectionMatrix() const {
        float left = -screenWidth / 2.0f / zoom;
        float right = screenWidth / 2.0f / zoom;
        float bottom = -screenHeight / 2.0f / zoom;
        float top = screenHeight / 2.0f / zoom;
        return glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    }

private:
    glm::vec2 position;
    float zoom;
    float screenWidth;
    float screenHeight;
};
#endif //CAMERA2D_H
