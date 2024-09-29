#include "Game.h"
#include <imgui.h>
#include <rlImGui.h>

void Game::DrawConnectModal() {
    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
                            ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));

    if (ImGui::Begin("Connect to Server", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::InputText("Host", connect_modal_host, IM_ARRAYSIZE(connect_modal_host));
        ImGui::InputText("Port", connect_modal_port, IM_ARRAYSIZE(connect_modal_port));

        if (ImGui::Button("Connect")) {
            ClientConnect(connect_modal_host, std::strtol(connect_modal_port, nullptr, 10));
        }

        if (!connect_modal_error_message.empty()) {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), connect_modal_error_message.c_str());
        }

        ImGui::End();
    }
}

void Game::DrawDebug() {
    ImGui::SetNextWindowPos(ImVec2(16.f, 8.f));
    ImGui::Begin("Debug", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("mpoxes");
    ImGui::Text(TextFormat("FPS: %d", GetFPS()));
    if (client_peer != nullptr) {
        if (ImGui::Button("Disconnect")) {
            ClientDisconnect();
        }
    }
    ImGui::Separator();
    for (const auto &p: players) {
        ImGui::Text(TextFormat("Player %d", p.first));
        ImGui::Text(TextFormat("Position: %f %f", p.second->position.x, p.second->position.y));
        ImGui::Text(TextFormat("Wish direction: %f %f", players_wish_dirs[p.first].x, players_wish_dirs[p.first].y));
        ImGui::Text(TextFormat("Color: %d %d %d", p.second->color.r, p.second->color.g, p.second->color.b));
        ImGui::Separator();
    }

    ImGui::End();
}

void Game::RenderImGui() {
    rlImGuiBegin();

    DrawDebug();

    if (client_peer == nullptr) {
        DrawConnectModal();
    }

    rlImGuiEnd();
}