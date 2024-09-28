#include <raylib.h>
#include <imgui.h>
#include <rlImGui.h>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <cstring>
#if defined(_WIN32)
#define NOGDI             // All GDI defines and routines
#define NOUSER            // All USER defines and routines
#endif
#include <enet/enet.h>
#if defined(_WIN32)           // raylib uses these names as function parameters
#undef near
#undef far
#endif
#include "Input.h"
#include "Player.h"
#include "Packets.h"

constexpr int DESIGN_WIDTH = 1000;
constexpr int DESIGN_HEIGHT = 562;

class Game {
public:
    bool isServer = false;
    Camera2D camera{};
    Player *player1 = nullptr;
    bool running = false;
    int idCounter = 0;
    std::unordered_map<int, Player *> playersMap;
    std::unordered_map<int, Vector2> playersWishDirMap;

    ENetHost *server{};
    ENetHost *client{};
    ENetPeer *client_peer{};

    void Setup();

    void Update(float dt);

    void Render();

    void RenderImGui();

    void Shutdown();
};

void Game::Setup() {
    running = true;
    TraceLog(LOG_INFO, "starting mpoxes.");
    if (!isServer) {
        InitWindow(DESIGN_WIDTH, DESIGN_HEIGHT, "mpoxes");

        Camera2D camera = {0};
        camera.zoom = 1.0f;

        rlImGuiSetup(true);
    }

    if (isServer) {
        ENetAddress address;

        address.host = ENET_HOST_ANY;
        address.port = 7777;

        server = enet_host_create(&address, 69, 2, 0, 0);

        if (server == nullptr) {
            TraceLog(LOG_FATAL, "An error occurred while trying to create an ENet server host.");
            exit(EXIT_FAILURE);
        }
    } else {
        ENetEvent event;
        ENetAddress address;

        client = enet_host_create(nullptr, 1, 2, 0, 0);

        if (client == nullptr) {
            TraceLog(LOG_FATAL, "An error occurred while trying to create an ENet client host.");
            exit(EXIT_FAILURE);
        }

        enet_address_set_host(&address, "localhost");
        address.port = 7777;

        client_peer = enet_host_connect(client, &address, 2, 0);

        if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
            TraceLog(LOG_INFO, "Connection to server succeeded.");
        } else {
            enet_peer_reset(client_peer);
            TraceLog(LOG_FATAL, "Connection to server failed.");
            exit(EXIT_FAILURE);
        }
    }
}

void Game::Update(float dt) {
    if (!isServer) {
        float scale = fmin(static_cast<float>(GetScreenWidth()) / DESIGN_WIDTH,
                           static_cast<float>(GetScreenHeight()) / DESIGN_HEIGHT);

        camera.offset.x = GetScreenWidth() / 2.0f;
        camera.offset.y = GetScreenHeight() / 2.0f;
        camera.zoom = scale;
    }

    for (auto p: playersMap) {
        p.second->Update(dt, playersWishDirMap[p.first]);
    }

    if (isServer) {
        ENetEvent event;
        while (enet_host_service(server, &event, 0) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT: {
                    TraceLog(LOG_INFO, "New client connected");
                    auto* p = new Player(++idCounter, Vector2(0, 0));
                    playersMap[p->id] = p;
                    playersWishDirMap[p->id] = Vector2(0, 0);
                    event.peer->data = reinterpret_cast<void *>(p->id);

                    for (auto& players_pair : playersMap) {
                        auto join_packet = server_join_packet(players_pair.second->id, players_pair.second->position);
                        enet_host_broadcast(server, 0, join_packet);
                    }

                    auto hello_packet = server_hello_packet(p->id);
                    enet_peer_send(event.peer, 0, hello_packet);
                    break;
                }
                case ENET_EVENT_TYPE_RECEIVE: {
                    auto pid = reinterpret_cast<int>(event.peer->data);
                    auto player = playersMap[pid];
                    auto data = reinterpret_cast<char*>(event.packet->data);
                    if (data[0] == 'm') {
                        auto wishDir = client_move_packet_get_input(data);
                        playersWishDirMap[pid] = moveDirectionFrom(wishDir);

                        auto bc_packet2 = server_move_packet(pid, wishDir);
                        enet_host_broadcast(server, 0, bc_packet2);
                    }

                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT: {
                    TraceLog(LOG_INFO, "Client disconnected");
                    event.peer->data = nullptr;
                    break;
                }
                case ENET_EVENT_TYPE_NONE:
                    break;
            }
        }
    } else {
        auto packet = client_move_packet(moveDirectionTo(Input::GetMoveDir()));
        enet_peer_send(client_peer, 0, packet);

        ENetEvent event;
        while (enet_host_service(client, &event, 0) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_RECEIVE: {
                    auto data = reinterpret_cast<char*>(event.packet->data);
                    if (data[0] == 'j') {
                        int id = server_join_packet_get_id(data);
                        Vector2 pos = server_join_packet_get_position(data);
                        Player* p = new Player(id, pos);
                        playersMap[id] = p;
                        playersWishDirMap[id] = Vector2(0,0);

                        TraceLog(LOG_INFO, TextFormat("New player joined: %d", id));
                    } else if (data[0] == 'm') {
                        int id = server_move_packet_get_id(data);
                        Vector2 wishDir = moveDirectionFrom(server_move_packet_get_input(data));

                        playersWishDirMap[id] = wishDir;
                    } else if (data[0] == 'h') {
                        auto pid = server_hello_packet_get_id(data);
                        player1 = playersMap[pid];
                    }

                    enet_packet_destroy(event.packet);
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT: {
                    TraceLog(LOG_FATAL, "Client disconnected");
                    exit(EXIT_FAILURE);
                    return;
                }
                case ENET_EVENT_TYPE_NONE:
                case ENET_EVENT_TYPE_CONNECT:
                    break;
            }
        }
    }
}

void Game::Render() {
    running = !WindowShouldClose();
    ClearBackground(BLACK);

    BeginMode2D(camera);
    for (auto p: playersMap) {
        p.second->Render();
    }

    EndMode2D();
}

void Game::RenderImGui() {
    rlImGuiBegin();

    auto drawList = ImGui::GetForegroundDrawList();

    int line = 0;
    drawList->AddText(ImVec2(16.f, ++line * 14.f), 0xFFFFFFFF, TextFormat("FPS: %d", GetFPS()));
    drawList->AddText(ImVec2(16.f, ++line * 14.f), 0xFFFFFFFF, TextFormat("----------"));
    for (auto p: playersMap) {
        drawList->AddText(ImVec2(16.f, ++line * 14.f), 0xFFFFFFFF, TextFormat("Player %d", p.first));
        drawList->AddText(ImVec2(16.f, ++line * 14.f), 0xFFFFFFFF,
                          TextFormat("Position: %f %f", p.second->position.x, p.second->position.y));
        drawList->AddText(ImVec2(16.f, ++line * 14.f), 0xFFFFFFFF,
                          TextFormat("Wishdir: %f %f", playersWishDirMap[p.first].x, playersWishDirMap[p.first].y));
        drawList->AddText(ImVec2(16.f, ++line * 14.f), 0xFFFFFFFF, TextFormat("----------"));
    }

    rlImGuiEnd();
}

void Game::Shutdown() {
    if (!isServer) {
        rlImGuiShutdown();
        CloseWindow();

        ENetEvent event;
        enet_peer_disconnect(client_peer, 0);

        while (enet_host_service(client, &event, 3000) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_RECEIVE:
                    enet_packet_destroy(event.packet);
                    break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    TraceLog(LOG_INFO, "Disconnection succeeded.");
                    return;
                case ENET_EVENT_TYPE_NONE:
                case ENET_EVENT_TYPE_CONNECT:
                    break;
            }
        }

        enet_peer_reset(client_peer);
        enet_host_destroy(client);
    }

    if (isServer) {
        enet_host_destroy(server);
    }
}

int main(int argc, char *argv[]) {
    Game game;

    if (argc > 1 && std::strcmp(argv[1], "server") == 0) {
        game.isServer = true;
    } else {
        game.isServer = false;
    }
    if (enet_initialize() != 0) {
        TraceLog(LOG_FATAL, "An error occurred while initializing ENet");
        return EXIT_FAILURE;
    }
    game.Setup();

    std::chrono::steady_clock::time_point lastFrameTime = std::chrono::steady_clock::now();

    while (game.running) {
        auto currentTime = std::chrono::steady_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
        lastFrameTime = currentTime;

        game.Update(deltaTime);

        if (!game.isServer) {
            BeginDrawing();
            game.Render();
            game.RenderImGui();
            EndDrawing();
        }
    }

    game.Shutdown();
    enet_deinitialize();
    return 0;
}
