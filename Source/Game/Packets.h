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

enum MoveDirection : uint8_t {
    UP = 0b0001,
    DOWN = 0b0010,
    LEFT = 0b0100,
    RIGHT = 0b1000
};

inline Vector2 moveDirectionFrom(uint8_t direction) {
    auto wishDir = Vector2(0, 0);

    if (direction & MoveDirection::UP) wishDir.y = 1.f;
    if (direction & MoveDirection::DOWN) wishDir.y = -1.f;
    if (direction & MoveDirection::RIGHT) wishDir.x = 1.f;
    if (direction & MoveDirection::LEFT) wishDir.x = -1.f;

    return Vector2Normalize(wishDir);
}

inline uint8_t moveDirectionTo(Vector2 direction) {
    uint8_t wishDir = 0;

    if (direction.y > 0.5f) wishDir |= MoveDirection::UP;
    if (direction.y < -0.5f) wishDir |= MoveDirection::DOWN;
    if (direction.x < -0.5f) wishDir |= MoveDirection::LEFT;
    if (direction.x > 0.5f) wishDir |= MoveDirection::RIGHT;

    return wishDir;
}

#pragma region client_move
inline ENetPacket *client_move_packet(uint8_t direction) {
    constexpr int packet_size = 1 + sizeof(Vector2);
    uint8_t data[packet_size] = {};
    data[0] = 'm';
    std::memcpy(data + 1, &direction, sizeof(direction));
    auto packet = enet_packet_create(data, packet_size, ENET_PACKET_FLAG_RELIABLE);
    return packet;
}

inline uint8_t client_move_packet_get_input(char *data) {
    uint8_t moveDir;
    std::memcpy(&moveDir, data + 1, sizeof(moveDir));
    return moveDir;
}
#pragma endregion
#pragma region server_move
inline ENetPacket *server_move_packet(int id, uint8_t moveDir) {
    constexpr int packet_size = 1 + sizeof(int) + sizeof(Vector2);
    uint8_t data[packet_size] = {};
    data[0] = 'm';
    std::memcpy(data + 1, &id, sizeof(id));
    std::memcpy(data + 1 + sizeof(int), &moveDir, sizeof(uint8_t));
    auto packet = enet_packet_create(data, packet_size, 0);
    return packet;
}

inline int server_move_packet_get_id(char *data) {
    int id;
    std::memcpy(&id, data + 1, sizeof(id));
    return id;
}

inline uint8_t server_move_packet_get_input(char *data) {
    uint8_t moveDir;
    std::memcpy(&moveDir, data + 1 + sizeof(int), sizeof(moveDir));
    return moveDir;
}
#pragma endregion
#pragma region server_join
inline ENetPacket *server_join_packet(int id, Vector2 position) {
    constexpr int packet_size = 1 + sizeof(int) + sizeof(Vector2);
    uint8_t data[packet_size] = {};
    data[0] = 'j';
    std::memcpy(data + 1, &id, sizeof(id));
    std::memcpy(data + 1 + sizeof(int), &position, sizeof(Vector2));
    auto packet = enet_packet_create(data, packet_size, 0);
    return packet;
}

inline int server_join_packet_get_id(char *data) {
    int id;
    std::memcpy(&id, data + 1, sizeof(id));
    return id;
}

inline Vector2 server_join_packet_get_position(char *data) {
    Vector2 position;
    std::memcpy(&position, data + 1 + sizeof(int), sizeof(position));
    return position;
}
#pragma endregion
#pragma region server_hello
inline ENetPacket *server_hello_packet(int id) {
    constexpr int packet_size = 1 + sizeof(int) + sizeof(Vector2);
    uint8_t data[packet_size] = {};
    data[0] = 'h';
    std::memcpy(data + 1, &id, sizeof(id));
    auto packet = enet_packet_create(data, packet_size, 0);
    return packet;
}

inline int server_hello_packet_get_id(char *data) {
    int id;
    std::memcpy(&id, data + 1, sizeof(id));
    return id;
}
#pragma endregion

#endif //PACKETS_H
