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
    bool is_server = false;
    Camera2D camera{};
    std::shared_ptr<Player> local_player;
    bool running = false;
    int id_counter = 0;
    std::unordered_map<int, std::shared_ptr<Player> > players;
    std::unordered_map<int, Vector2> players_wish_dirs;

    ENetHost *server{};
    ENetHost *client{};
    ENetPeer *client_peer{};
    std::string connect_modal_error_message;
    char connect_modal_host[256] = "localhost";
    char connect_modal_port[6] = "7777";

    void Setup();

    void Update(float dt);

    void Render();

    void RenderImGui();

    void Shutdown();

    void ClientConnect(std::string host, int port);

    void ClientDisconnect();
};

void Game::Setup() {
    running = true;
    TraceLog(LOG_INFO, "starting mpoxes.");
    if (!is_server) {
        InitWindow(DESIGN_WIDTH, DESIGN_HEIGHT, "mpoxes");

        Camera2D camera = {0};
        camera.zoom = 1.0f;

        rlImGuiSetup(true);
    }

    if (is_server) {
        ENetAddress address;

        address.host = ENET_HOST_ANY;
        address.port = 7777;

        server = enet_host_create(&address, 69, 2, 0, 0);

        if (server == nullptr) {
            TraceLog(LOG_FATAL, "An error occurred while trying to create an ENet server host.");
            exit(EXIT_FAILURE);
        }
    } else {
        client = enet_host_create(nullptr, 1, 2, 0, 0);

        if (client == nullptr) {
            TraceLog(LOG_FATAL, "An error occurred while trying to create an ENet client host.");
            exit(EXIT_FAILURE);
        }
    }
}

void Game::Update(float dt) {
    if (!is_server) {
        float scale = fmin(static_cast<float>(GetScreenWidth()) / DESIGN_WIDTH,
                           static_cast<float>(GetScreenHeight()) / DESIGN_HEIGHT);

        camera.offset.x = GetScreenWidth() / 2.0f;
        camera.offset.y = GetScreenHeight() / 2.0f;
        camera.zoom = scale;
    }

    for (auto p: players) {
        p.second->Update(dt, players_wish_dirs[p.first]);
    }

    if (is_server) {
        ENetEvent event;
        while (enet_host_service(server, &event, 0) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT: {
                    TraceLog(LOG_INFO, "New client connected");
                    auto p = std::make_shared<Player>(++id_counter, Vector2(0, 0));
                    players[p->id] = p;
                    players_wish_dirs[p->id] = Vector2(0, 0);
                    event.peer->data = reinterpret_cast<void *>(p->id);

                    for (auto &players_pair: players) {
                        auto join_packet = server_join_packet(players_pair.second->id, players_pair.second->position);
                        enet_host_broadcast(server, 0, join_packet);
                    }

                    auto hello_packet = server_hello_packet(p->id);
                    enet_peer_send(event.peer, 0, hello_packet);
                    break;
                }
                case ENET_EVENT_TYPE_RECEIVE: {
                    auto pid = reinterpret_cast<int>(event.peer->data);
                    auto player = players[pid];
                    auto data = reinterpret_cast<char *>(event.packet->data);
                    if (data[0] == 'm') {
                        auto wishDir = client_move_packet_get_input(data);
                        players_wish_dirs[pid] = moveDirectionFrom(wishDir);

                        auto bc_packet2 = server_move_packet(pid, wishDir, player->position);
                        enet_host_broadcast(server, 0, bc_packet2);
                    }

                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT: {
                    auto pid = reinterpret_cast<int>(event.peer->data);
                    TraceLog(LOG_INFO, "Client disconnected");
                    auto bc_packet = server_bye_packet(pid);
                    enet_host_broadcast(server, 0, bc_packet);
                    event.peer->data = nullptr;
                    break;
                }
                case ENET_EVENT_TYPE_NONE:
                    break;
            }
        }
    } else {
        if (client_peer != nullptr) {
            auto packet = client_move_packet(moveDirectionTo(Input::GetMoveDir()));
            enet_peer_send(client_peer, 0, packet);

            ENetEvent event;
            while (enet_host_service(client, &event, 0) > 0) {
                switch (event.type) {
                    case ENET_EVENT_TYPE_RECEIVE: {
                        auto data = reinterpret_cast<char *>(event.packet->data);
                        if (data[0] == 'j') {
                            int id = server_join_packet_get_id(data);
                            Vector2 pos = server_join_packet_get_position(data);
                            auto p = std::make_shared<Player>(id, pos);
                            players[id] = p;
                            players_wish_dirs[id] = Vector2(0, 0);

                            TraceLog(LOG_INFO, TextFormat("New player joined: %d", id));
                        } else if (data[0] == 'm') {
                            int id = server_move_packet_get_id(data);
                            Vector2 wishDir = moveDirectionFrom(server_move_packet_get_input(data));
                            Vector2 newPosition = server_move_packet_get_position(data);

                            players_wish_dirs[id] = wishDir;
                            players[id]->position = newPosition;
                        } else if (data[0] == 'h') {
                            auto pid = server_hello_packet_get_id(data);
                            local_player = players[pid];
                        } else if (data[0] == 'b') {
                            auto pid = server_bye_packet_get_id(data);
                            auto player = players[pid];

                            if (player->id == local_player->id) {
                                ClientDisconnect();
                            } else {
                                players.erase(pid);
                            }
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
}

void Game::Render() {
    running = !WindowShouldClose();
    ClearBackground(BLACK);

    BeginMode2D(camera);
    for (auto p: players) {
        p.second->Render();
    }

    EndMode2D();
}

void Game::RenderImGui() {
    rlImGuiBegin();

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
    for (auto p: players) {
        ImGui::Text(TextFormat("Player %d", p.first));
        ImGui::Text(TextFormat("Position: %f %f", p.second->position.x, p.second->position.y));
        ImGui::Text(TextFormat("Wishdir: %f %f", players_wish_dirs[p.first].x, players_wish_dirs[p.first].y));
        ImGui::Separator();
    }

    ImGui::End();

    if (client_peer == nullptr) {
        ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
                                ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));

        if (ImGui::Begin("Connect to Server", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::InputText("Host", connect_modal_host, IM_ARRAYSIZE(connect_modal_host));
            ImGui::InputText("Port", connect_modal_port, IM_ARRAYSIZE(connect_modal_port));

            if (ImGui::Button("Connect")) {
                ClientConnect(connect_modal_host, std::atoi(connect_modal_port));
            }

            if (!connect_modal_error_message.empty()) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), connect_modal_error_message.c_str());
            }

            ImGui::End();
        }
    }


    rlImGuiEnd();
}

void Game::Shutdown() {
    if (!is_server) {
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

    if (is_server) {
        enet_host_destroy(server);
    }
}

void Game::ClientConnect(std::string host, int port) {
    ENetEvent event;
    ENetAddress address;

    enet_address_set_host(&address, host.c_str());
    address.port = port;

    client_peer = enet_host_connect(client, &address, 2, 0);

    if (enet_host_service(client, &event, 1000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        TraceLog(LOG_INFO, "Connection to server succeeded.");
    } else {
        enet_peer_reset(client_peer);
        client_peer = nullptr;
        connect_modal_error_message = "Could not connect to server.";
        TraceLog(LOG_ERROR, "Connection to server failed.");
    }
}

void Game::ClientDisconnect() {
    TraceLog(LOG_INFO, "dISCONNECTING");
    local_player = nullptr;
    id_counter = 0;
    players.clear();
    players_wish_dirs.clear();
    enet_peer_disconnect(client_peer, 0);
    enet_host_destroy(client);
    client_peer = nulwlptr;
    client = nullptr;
    connect_modal_error_message = "";
}

int main(int argc, char *argv[]) {
    Game game;

    if (argc > 1 && std::strcmp(argv[1], "server") == 0) {
        game.is_server = true;
    } else {
        game.is_server = false;
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

        if (!game.is_server) {
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
