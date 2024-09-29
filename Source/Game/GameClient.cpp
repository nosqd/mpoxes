#include "Game.h"
#include "Packets.h"
#include "Input.h"

void Game::HandleClientNetwork() {
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
                        Color color = server_join_packet_get_color(data);
                        auto p = std::make_shared<Player>(id, pos, color);
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
                }
                case ENET_EVENT_TYPE_NONE:
                case ENET_EVENT_TYPE_CONNECT:
                    break;
            }
        }
    }
}

void Game::SetupClientNetwork() {
    client = enet_host_create(nullptr, 1, 2, 0, 0);

    if (client == nullptr) {
        TraceLog(LOG_FATAL, "An error occurred while trying to create an ENet client host.");
        exit(EXIT_FAILURE);
    }
}

void Game::ClientConnect(const std::string &host, int port) {
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
    local_player = nullptr;
    id_counter = 0;
    players.clear();
    players_wish_dirs.clear();
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

    enet_host_destroy(client);
    client_peer = nullptr;
    client = nullptr;
    connect_modal_error_message = "";
}
