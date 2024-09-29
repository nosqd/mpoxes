#ifndef GAME_H
#define GAME_H

#include <enet/enet.h>
#include <unordered_map>
#include <memory>
#include <string>
#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>

#include "Player.h"
#include "../Math/Vector.h"
#include "../Math/Color.h"
#include "../Render/Camera2D.h"
#include "../Render/Shader.h"

constexpr int DESIGN_WIDTH = 1000;
constexpr int DESIGN_HEIGHT = 562;

class Game {
public:
    GLFWwindow * window;

    bool running = false;
    int id_counter = 0;

    std::shared_ptr<Player> local_player;
    std::unordered_map<int, std::shared_ptr<Player> > players;
    std::unordered_map<int, Vector2> players_wish_dirs;

    ENetHost *server{};
    ENetHost *client{};
    ENetPeer *client_peer{};

    Camera2D camera{0,0};
    Shader shader{};

    std::string connect_modal_error_message;

    void Setup();
    void Update(float dt);
    void Shutdown();
#ifdef SERVER
    void HandleServerNetwork();
    void StartServer();
#endif
#ifdef CLIENT
    void SetupClientNetwork();
    void HandleClientNetwork();
    void Render();
    void DrawConnectModal();
    void DrawDebug(float dt);
    void RenderImGui(float dt);
    void ClientConnect(const std::string &host, int port);
    void ClientDisconnect();
#endif
};

#endif //GAME_H
