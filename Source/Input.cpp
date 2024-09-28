//
// Created by null on 9/28/2024.
//

#include "Input.h"

Vector2 Input::GetMoveDir() {
    Vector2 wish_dir = { 0.0f, 0.0f };

    if (IsKeyDown(KEY_W)) wish_dir.y -= 1.0f;
    if (IsKeyDown(KEY_S)) wish_dir.y += 1.0f;
    if (IsKeyDown(KEY_A)) wish_dir.x -= 1.0f;
    if (IsKeyDown(KEY_D)) wish_dir.x += 1.0f;

    // Normalize the vector if it's not zero
    if (wish_dir.x != 0.0f || wish_dir.y != 0.0f) {
        float length = sqrtf(wish_dir.x * wish_dir.x + wish_dir.y * wish_dir.y);
        wish_dir.x /= length;
        wish_dir.y /= length;
    }

    return wish_dir;
}
