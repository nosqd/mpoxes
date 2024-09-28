//
// Created by null on 9/28/2024.
//

#include "Packets.h"
#include <iostream>

void suite1() {
    Vector2 input_expected = Vector2(1.f, -1.f);
    auto packet = client_move_packet(input_expected);

    std::cout << "Client Move Packet test suite\n";

    auto input = client_move_packet_get_input(reinterpret_cast<char *>(packet->data));
    if (input_expected.x == input.x && input_expected.y == input.y) {
        std::cout << "input: OK\n";
    }
    else {
        std::cout << "input: FAIL\n";
        std::cout << "x: expected: " << input_expected.x << " got: " << input.x << "\n";
        std::cout << "y: expected: " << input_expected.y << " got: " << input.y << "\n";
    }
}

void suite2() {
    int id_expected = 69;
    Vector2 input_expected = Vector2(1.f, -1.f);
    auto packet = server_move_packet(id_expected, input_expected);

    std::cout << "Server Move Packet test suite\n";

    auto id = server_move_packet_get_id(reinterpret_cast<char *>(packet->data));
    if (id_expected == id) {
        std::cout << "id: OK\n";
    }
    else {
        std::cout << "id: FAIL\n";
        std::cout << "expected: " << id_expected << " got: " << id << "\n";
    }

    auto input = server_move_packet_get_input(reinterpret_cast<char *>(packet->data));
    if (input_expected.x == input.x && input_expected.y == input.y) {
        std::cout << "input: OK\n";
    }
    else {
        std::cout << "input: FAIL\n";
        std::cout << "x: expected: " << input_expected.x << " got: " << input.x << "\n";
        std::cout << "y: expected: " << input_expected.y << " got: " << input.y << "\n";
    }
}

void suite3() {
    int id_expected = 69;
    Vector2 position_expected = Vector2(50.2f, -320.1f);
    auto packet = server_join_packet(id_expected, position_expected);

    std::cout << "Server Join Packet test suite\n";

    auto id = server_join_packet_get_id(reinterpret_cast<char *>(packet->data));
    if (id_expected == id) {
        std::cout << "id: OK\n";
    }
    else {
        std::cout << "id: FAIL\n";
        std::cout << "expected: " << id_expected << " got: " << id << "\n";
    }

    auto input = server_join_packet_get_position(reinterpret_cast<char *>(packet->data));
    if (position_expected.x == input.x && position_expected.y == input.y) {
        std::cout << "position: OK\n";
    }
    else {
        std::cout << "position: FAIL\n";
        std::cout << "x: expected: " << position_expected.x << " got: " << input.x << "\n";
        std::cout << "y: expected: " << position_expected.y << " got: " << input.y << "\n";
    }
}

int main() {
    suite1();
    std::cout << "--------\n";
    suite2();
    std::cout << "--------\n";
    suite3();
    std::cout << "--------\n";
    return 0;
}
