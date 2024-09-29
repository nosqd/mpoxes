#ifndef GAME_H

#define GAME_H
#if defined(_WIN32)
#define NOGDI             // All GDI defines and routines
#define NOUSER            // All USER defines and routines
#endif
#include <enet/enet.h>
#if defined(_WIN32)           // raylib uses these names as function parameters
#undef near
#undef far
#undef PlaySound
#endif
#include <raylib.h>
#include <unordered_map>
#include <memory>
#include <string>
#include "Player.h"

constexpr int DESIGN_WIDTH = 1000;
constexpr int DESIGN_HEIGHT = 562;

class Game {
public:
    bool is_server = false;
    Camera2D camera{};
    bool running = false;
    int id_counter = 0;

    std::shared_ptr<Player> local_player;
    std::unordered_map<int, std::shared_ptr<Player> > players;
    std::unordered_map<int, Vector2> players_wish_dirs;

    ENetHost *server{};
    ENetHost *client{};
    ENetPeer *client_peer{};

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

    void DrawDebug();

    void RenderImGui();

    void Shutdown();

    void ClientConnect(const std::string &host, int port);

    void ClientDisconnect();
};

#endif //GAME_H
