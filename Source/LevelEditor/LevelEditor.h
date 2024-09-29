//
// Created by null on 9/29/2024.
//

#ifndef LEVELEDITOR_H
#define LEVELEDITOR_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Level/Level.h"
#include "../Level/Wall.h"
#include "../Render/Shader.h"
#include "../Render/Camera2D.h"
#include <memory>
#include <unordered_map>
#include <imgui.h>

class LevelEditor {
public:
    GLFWwindow* window;

    std::shared_ptr<Level> level;
    std::shared_ptr<Wall> selectedWall;
    std::unordered_map<int, bool> walls_window_state;
    Shader shader{};
    Camera2D camera{0,0};

    explicit LevelEditor(GLFWwindow* window);

    void Setup();
    void Frame();
    void Draw();
    void DrawImGui();
};
#endif //LEVELEDITOR_H
