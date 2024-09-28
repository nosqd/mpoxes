#define CATCH_CONFIG_MAIN
#include "catch2/catch_all.hpp"
#include "../Game/Packets.h"
#include <random>

std::random_device rd;
std::mt19937 gen(rd());

int randomInt(int min, int max) {
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

float randomFloat(float min, float max) {
    std::uniform_real_distribution<> distrib(min, max);
    return static_cast<float>(distrib(gen));
}

Vector2 randomVector2(float min, float max) {
    return Vector2(randomFloat(min, max), randomFloat(min, max));
}

bool vector2Equal(const Vector2& a, const Vector2& b, float epsilon = 0.0001f) {
    return std::abs(a.x - b.x) < epsilon && std::abs(a.y - b.y) < epsilon;
}

TEST_CASE("Client Move Packet", "[client][move]") {
    Vector2 inputExpected = randomVector2(-1000.0f, 1000.0f);

    SECTION("Input is correctly set and retrieved") {
        auto packet = client_move_packet(inputExpected);
        auto input = client_move_packet_get_input(reinterpret_cast<char*>(packet->data));
        REQUIRE(vector2Equal(input, inputExpected));
    }
}

TEST_CASE("Server Move Packet", "[server][move]") {
    int idExpected = randomInt(1, 1000000);
    Vector2 inputExpected = randomVector2(-1000.0f, 1000.0f);

    SECTION("ID and Input are correctly set and retrieved") {
        auto packet = server_move_packet(idExpected, inputExpected);

        auto id = server_move_packet_get_id(reinterpret_cast<char*>(packet->data));
        REQUIRE(id == idExpected);

        auto input = server_move_packet_get_input(reinterpret_cast<char*>(packet->data));
        REQUIRE(vector2Equal(input, inputExpected));
    }
}

TEST_CASE("Server Join Packet", "[server][join]") {
    int idExpected = randomInt(1, 1000000);
    Vector2 positionExpected = randomVector2(-10000.0f, 10000.0f);

    SECTION("ID and Position are correctly set and retrieved") {
        auto packet = server_join_packet(idExpected, positionExpected);

        auto id = server_join_packet_get_id(reinterpret_cast<char*>(packet->data));
        REQUIRE(id == idExpected);

        auto position = server_join_packet_get_position(reinterpret_cast<char*>(packet->data));
        REQUIRE(vector2Equal(position, positionExpected));
    }
}