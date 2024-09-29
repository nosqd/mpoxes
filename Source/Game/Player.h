//
// Created by null on 9/28/2024.
//

#ifndef PLAYER_H
#define PLAYER_H
#include "../Math/Vector.h"
#include "../Math/Color.h"
#include "../Render/Mesh.h"
#include "../Render/Shader.h"
#include "../Render/Texture.h"

constexpr int PLAYER_SPEED = 32.f;
static inline Vector2 PLAYER_SIZE = Vector2(16.f, 16.f);

class Player {
public:
    int id;
    Vector2 position{};
    Color color{};
    Mesh mesh{};
    Texture texture;

    Player(int id, Vector2 position, Color color);
    void Update(float dt, Vector2 move_dir);
    void Render(Shader shader) ;
};
#endif //PLAYER_H
