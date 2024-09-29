#include "LevelEditor.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <tinyfiledialogs/tinyfiledialogs.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>
#include "../Level/LevelLoader.h"
#include "../Math/WorldToScreen.h"

LevelEditor::LevelEditor(GLFWwindow *window) {
    this->window = window;
}

void LevelEditor::Setup() {
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    shader = Shader("Shaders/shader.vert", "Shaders/shader.frag");
    camera = Camera2D(width, height);
}

void LevelEditor::Frame() {
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    DrawImGui();

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    Draw();

    glfwSwapBuffers(window);
}

void LevelEditor::Draw() {
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    shader.use();
    auto proj = camera.getProjectionMatrix();
    auto view = camera.getViewMatrix();
    shader.setMat4("projection", proj);
    shader.setMat4("view", view);

    if (level) {
        auto drawList = ImGui::GetForegroundDrawList();
        drawList->AddText(ImVec2(16, 16), 0xFF00FF00, "mpoxes-editor");
        for (auto wall: level->walls) {
            wall->Render(shader);
        }
    }
}

void LevelEditor::DrawImGui() {
    ImGui::Begin("Current Level");

    if (level) {
        ImGui::Text("Current Level has %d walls", level->walls.size());
        if (ImGui::Button("Add Wall")) {
            level->walls.push_back(new Wall(level->walls.size() + 1, Vector2(0, 0), Vector2(10, 10),
                                            Color(255, 255, 255, 255)));
        }
        if (ImGui::Button("Save Level")) {
            char const *const patterns[] = {"*.mlvl"};
            auto file = tinyfd_saveFileDialog(
                "Select Level file",
                "",
                1,
                patterns,
                "mpoxes levels (.mlvl)"
            );

            if (file != nullptr)
                LevelLoader::SaveToFile(file, level.get());
        }
        if (ImGui::Button("Clear Level")) {
            level = nullptr;
        }
    } else {
        if (ImGui::Button("Load")) {
            char const *const patterns[] = {"*.mlvl"};
            auto file = tinyfd_openFileDialog(
                "Select Level file",
                "",
                1,
                patterns,
                "mpoxes levels (.mlvl)",
                0
            );
            if (file != nullptr) {
                spdlog::info("Loading level {}", file);
                auto level = LevelLoader::LoadFromFile(file);
                if (level) {
                    this->level = std::shared_ptr<Level>(level);
                }
            }
        }
        if (ImGui::Button("Create Level")) {
            this->level = std::make_shared<Level>();
        }
    }

    ImGui::End();

    if (level) {
        auto drawList = ImGui::GetForegroundDrawList();
        auto view = camera.getViewMatrix();
        auto proj = camera.getProjectionMatrix();
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        drawList->AddText(ImVec2(16, 16), 0xFF00FF00, "mpoxes-editor");

        size_t i = 0;
        for (auto wall: level->walls) {
            float position[] = {wall->position.x, wall->position.y};
            float size[] = {wall->size.x, wall->size.y};
            float color[] = {wall->color.r / 255, wall->color.g / 255, wall->color.b / 255};

            if (!this->walls_window_state.contains(wall->id)) {
                this->walls_window_state[wall->id] = false;
            }

            if (this->walls_window_state[wall->id]) {
                ImGui::Begin(fmt::format("Wall {}", wall->id).c_str(), nullptr,
                             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

                ImGui::Text("VAO: %d | Texture Id: %d", wall->mesh.VAO, wall->texture.ID);
                if (ImGui::InputFloat2("Position", position)) {
                    level->walls[i]->position = Vector2(position[0], position[1]);
                }
                if (ImGui::InputFloat2("Size", size)) {
                    level->walls[i]->size = Vector2(size[0], size[1]);
                }
                if (ImGui::ColorEdit3("Color", color)) {
                    level->walls[i]->color = Color(color[0] * 255, color[1] * 255, color[2] * 255, 255);
                }
                if (ImGui::Button("Reapply color")) {
                    level->walls[i]->ReapplyColor();
                }

                ImGui::End();
            }

            Vector2 screenPos1{};
            Vector2 screenPos2{};

            wall->GetScreenBounds(view, proj, width, height, screenPos1, screenPos2);

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                auto mouse = ImGui::GetMousePos();

                float minX = std::min(screenPos1.x, screenPos2.x);
                float maxX = std::max(screenPos1.x, screenPos2.x);
                float minY = std::min(screenPos1.y, screenPos2.y);
                float maxY = std::max(screenPos1.y, screenPos2.y);

                if (minX <= mouse.x && mouse.x <= maxX &&
                    minY <= mouse.y && mouse.y <= maxY) {
                    this->walls_window_state[wall->id] = !this->walls_window_state[wall->id];
                }
            }

            i++;
        }
    }
}
