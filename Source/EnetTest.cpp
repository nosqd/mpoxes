#include <enet/enet.h>
#include <iostream>
#include <string>
#include <cstring>

const uint16_t PORT = 7777;
const size_t MAX_CLIENTS = 32;

void server() {
    ENetAddress address;
    ENetHost* server;
    
    address.host = ENET_HOST_ANY;
    address.port = PORT;

    server = enet_host_create(&address, MAX_CLIENTS, 2, 0, 0);
    
    if (server == nullptr) {
        std::cerr << "An error occurred while trying to create an ENet server host.\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "Server started on port " << PORT << std::endl;

    ENetEvent event;
    while (true) {
        while (enet_host_service(server, &event, 1000) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    std::cout << "A new client connected from " 
                              << event.peer->address.host << ":" << event.peer->address.port << std::endl;
                    break;
                case ENET_EVENT_TYPE_RECEIVE:
                    std::cout << "Message received: " << event.packet->data << std::endl;
                    enet_peer_send(event.peer, 0, event.packet);
                    break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    std::cout << "Client disconnected" << std::endl;
                    event.peer->data = nullptr;
                    break;
                case ENET_EVENT_TYPE_NONE:
                    break;
            }
        }
    }

    enet_host_destroy(server);
}

void client() {
    ENetHost* client;
    ENetAddress address;
    ENetPeer* peer;
    ENetEvent event;

    client = enet_host_create(nullptr, 1, 2, 0, 0);
    
    if (client == nullptr) {
        std::cerr << "An error occurred while trying to create an ENet client host.\n";
        exit(EXIT_FAILURE);
    }

    enet_address_set_host(&address, "localhost");
    address.port = PORT;

    peer = enet_host_connect(client, &address, 2, 0);
    
    if (peer == nullptr) {
        std::cerr << "No available peers for initiating an ENet connection.\n";
        exit(EXIT_FAILURE);
    }

    if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        std::cout << "Connection to server succeeded." << std::endl;
    } else {
        enet_peer_reset(peer);
        std::cerr << "Connection to server failed." << std::endl;
        exit(EXIT_FAILURE);
    }

    while (true) {
        std::string message;
        std::cout << "Enter message (or 'quit' to exit): ";
        std::getline(std::cin, message);

        if (message == "quit") break;

        ENetPacket* packet = enet_packet_create(message.c_str(), message.length() + 1, ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(peer, 0, packet);

        while (enet_host_service(client, &event, 1000) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_RECEIVE:
                    std::cout << "Server response: " << event.packet->data << std::endl;
                    enet_packet_destroy(event.packet);
                    break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    std::cout << "Disconnected from server" << std::endl;
                    return;
                case ENET_EVENT_TYPE_NONE:
                case ENET_EVENT_TYPE_CONNECT:
                    break;
            }
        }
    }

    enet_peer_disconnect(peer, 0);

    while (enet_host_service(client, &event, 3000) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Disconnection succeeded." << std::endl;
                return;
            case ENET_EVENT_TYPE_NONE:
            case ENET_EVENT_TYPE_CONNECT:
                break;
        }
    }

    enet_peer_reset(peer);
    enet_host_destroy(client);
}

int main(int argc, char *argv[]) {
    bool isServer = false;
    if (argc > 1 && std::strcmp(argv[1], "server") == 0) {
        isServer = true;
    } else {
        isServer = false;
    }

    if (enet_initialize() != 0) {
        fprintf(stderr, "An error occurred while initializing ENet.\n");
        return EXIT_FAILURE;
    }
    if (isServer) server();
    else client();
    atexit(enet_deinitialize);
}