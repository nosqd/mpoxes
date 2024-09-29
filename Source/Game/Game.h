#ifndef GAME_H
#define GAME_H

#include <enet/enet.h>
#include <unordered_map>
#include <memory>
#include <string>
#include <spdlog/spdlog.h>
#include "Player.h"
#include "Vector.h"
#include "Color.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Camera2D.h"

constexpr int DESIGN_WIDTH = 1000;
constexpr int DESIGN_HEIGHT = 562;

class Game {
public:
    GLFWwindow * window;

    bool is_server = false;
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
    char connect_modal_host[256] = "localhost";
    char connect_modal_port[6] = "7777";

    void StartServer();

    void SetupClientNetwork();

    void Setup();

    void HandleServerNetwork();

    void HandleClientNetwork();

    void Update(float dt);

    void Render();

    void DrawConnectModal();

    void DrawDebug(float dt);

    void RenderImGui(float dt);

    void Shutdown();

    void ClientConnect(const std::string &host, int port);

    void ClientDisconnect();
};

#endif //GAME_H
