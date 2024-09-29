#include "Game.h"
#include "Packets.h"

void Game::StartServer() {
    ENetAddress address;

    address.host = ENET_HOST_ANY;
    address.port = 7777;

    server = enet_host_create(&address, 69, 2, 0, 0);

    if (server == nullptr) {
        TraceLog(LOG_FATAL, "An error occurred while trying to create an ENet server host.");
        exit(EXIT_FAILURE);
    }
}

void Game::HandleServerNetwork() {
    ENetEvent event;
    while (enet_host_service(server, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                TraceLog(LOG_INFO, "New client connected");
                auto c = Color(GetRandomValue(127, 255), GetRandomValue(127, 255), GetRandomValue(127, 255), 255);
                auto p = std::make_shared<Player>(++id_counter, Vector2(0, 0), c);
                players[p->id] = p;
                players_wish_dirs[p->id] = Vector2(0, 0);
                event.peer->data = reinterpret_cast<void *>(p->id);

                for (const auto &players_pair: players) {
                    auto join_packet = server_join_packet(players_pair.second->id, players_pair.second->position,
                                                          players_pair.second->color);
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
}
