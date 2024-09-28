//
// Created by null on 9/28/2024.
//

#ifndef PACKETS_H
#define PACKETS_H
#include <cstdint>
#include <enet/enet.h>
#if defined(_WIN32)           // raylib uses these names as function parameters
#undef near
#undef far
#endif
#include <raymath.h>
#include <cstdint>
#include <cstring>
#include <iostream>

inline ENetPacket *client_move_packet(Vector2 moveDir) {
    constexpr int packet_size = 1 + sizeof(Vector2);
    uint8_t data[packet_size] = {};
    data[0] = 'm';
    Vector2 input = moveDir;
    std::memcpy(data + 1, &input, sizeof(Vector2));
    auto packet = enet_packet_create(data, packet_size, ENET_PACKET_FLAG_RELIABLE);
    return packet;
}

inline Vector2 client_move_packet_get_input(char* data) {
    Vector2 moveDir;
    std::memcpy(&moveDir, data + 1, sizeof(moveDir));
    return moveDir;
}

inline ENetPacket *server_move_packet(int id, Vector2 moveDir) {
    constexpr int packet_size = 1 + sizeof(int) + sizeof(Vector2);
    uint8_t data[packet_size] = {};
    data[0] = 'm';
    std::memcpy(data + 1, &id, sizeof(id));
    Vector2 input = moveDir;
    std::memcpy(data + 1 + sizeof(int), &input, sizeof(Vector2));
    auto packet = enet_packet_create(data, packet_size, 0);
    return packet;
}

inline int server_move_packet_get_id(char* data) {
    int id;
    std::memcpy(&id, data + 1, sizeof(id));
    return id;
}

inline Vector2 server_move_packet_get_input(char* data) {
    Vector2 moveDir;
    std::memcpy(&moveDir, data + 1 + sizeof(int), sizeof(moveDir));
    return moveDir;
}

inline ENetPacket *server_join_packet(int id, Vector2 position) {
    constexpr int packet_size = 1 + sizeof(int) + sizeof(Vector2);
    uint8_t data[packet_size] = {};
    data[0] = 'j';
    std::memcpy(data + 1, &id, sizeof(id));
    std::memcpy(data + 1 + sizeof(int), &position, sizeof(Vector2));
    auto packet = enet_packet_create(data, packet_size, 0);
    return packet;
}

inline int server_join_packet_get_id(char* data) {
    int id;
    std::memcpy(&id, data + 1, sizeof(id));
    return id;
}

inline Vector2 server_join_packet_get_position(char* data) {
    Vector2 position;
    std::memcpy(&position, data + 1 + sizeof(int), sizeof(position));
    return position;
}

inline ENetPacket *server_hello_packet(int id) {
    constexpr int packet_size = 1 + sizeof(int) + sizeof(Vector2);
    uint8_t data[packet_size] = {};
    data[0] = 'h';
    std::memcpy(data + 1, &id, sizeof(id));
    auto packet = enet_packet_create(data, packet_size, 0);
    return packet;
}

inline int server_hello_packet_get_id(char* data) {
    int id;
    std::memcpy(&id, data + 1, sizeof(id));
    return id;
}

#endif //PACKETS_H
