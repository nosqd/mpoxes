#define CATCH_CONFIG_MAIN
#include "catch2/catch_all.hpp"
#include "../Game/Packets.h"
#include "../Game/Vector.h"
#include "../Game/Random.h"
#include <random>


bool vector2Equal(const Vector2 &a, const Vector2 &b, float epsilon = 0.0001f) {
    return std::abs(a.x - b.x) < epsilon && std::abs(a.y - b.y) < epsilon;
}

TEST_CASE("Client Move Packet", "[client][move]") {
    Vector2 inputExpected = Vector2(1.f, -1.f).normalized();

    SECTION("Input is correctly set and retrieved") {
        auto packet = client_move_packet(moveDirectionTo(inputExpected));
        auto input = client_move_packet_get_input(reinterpret_cast<char *>(packet->data));
        REQUIRE(vector2Equal(moveDirectionFrom(input), inputExpected));
    }
}

TEST_CASE("Server Move Packet", "[server][move]") {
    int idExpected = randomInt(1, 1000000);
    Vector2 inputExpected = Vector2(1.f, -1.f).normalized();
    Vector2 positionExpected = randomVector2(-10000.0f, 10000.0f);


    SECTION("ID and Input are correctly set and retrieved") {
        auto packet = server_move_packet(idExpected, moveDirectionTo(inputExpected), positionExpected);

        auto id = server_move_packet_get_id(reinterpret_cast<char *>(packet->data));
        REQUIRE(id == idExpected);

        auto input = server_move_packet_get_input(reinterpret_cast<char *>(packet->data));
        REQUIRE(vector2Equal(moveDirectionFrom(input), inputExpected));

        auto position = server_move_packet_get_position(reinterpret_cast<char *>(packet->data));
        REQUIRE(vector2Equal(position, positionExpected));
    }
}

TEST_CASE("Server Join Packet", "[server][join]") {
    int idExpected = randomInt(1, 1000000);
    Vector2 positionExpected = randomVector2(-10000.0f, 10000.0f);
    Color colorExpected = Color(69, 420, 52, 255);

    SECTION("ID and Position are correctly set and retrieved") {
        auto packet = server_join_packet(idExpected, positionExpected, colorExpected);

        auto id = server_join_packet_get_id(reinterpret_cast<char *>(packet->data));
        REQUIRE(id == idExpected);

        auto position = server_join_packet_get_position(reinterpret_cast<char *>(packet->data));
        REQUIRE(vector2Equal(position, positionExpected));

        auto color = server_join_packet_get_color(reinterpret_cast<char *>(packet->data));
        REQUIRE((color.r == colorExpected.r && color.g == colorExpected.g && color.b == colorExpected.b && color.a == colorExpected.a));
    }
}

TEST_CASE("Server Bye Packet", "[server][bye]") {
    int idExpected = randomInt(1, 1000000);

    SECTION("ID are correctly set and retrieved") {
        auto packet = server_bye_packet(idExpected);

        auto id = server_bye_packet_get_id(reinterpret_cast<char *>(packet->data));
        REQUIRE(id == idExpected);
    }
}

TEST_CASE("Server Hello Packet", "[server][hello]") {
    int idExpected = randomInt(1, 1000000);

    SECTION("ID are correctly set and retrieved") {
        auto packet = server_hello_packet(idExpected);

        auto id = server_hello_packet_get_id(reinterpret_cast<char *>(packet->data));
        REQUIRE(id == idExpected);
    }
}


TEST_CASE("Move Direction Convertor", "[misc][movement]") {
    Vector2 inputExpected = Vector2(1.f, -1.f).normalized();
    SECTION("Input is correctly converted back and forth") {
        auto numeric = moveDirectionTo(inputExpected);

        REQUIRE(vector2Equal(moveDirectionFrom(numeric), inputExpected));
    }
}
