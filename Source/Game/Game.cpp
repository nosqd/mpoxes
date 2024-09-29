#include "Game.h"
#include <rlImGui.h>

void Game::Setup() {
    running = true;

    TraceLog(LOG_INFO, "starting mpoxes.");
    SetRandomSeed(52);

    if (!is_server) {
        InitWindow(DESIGN_WIDTH, DESIGN_HEIGHT, "mpoxes");

        Camera2D camera = {0};
        camera.zoom = 1.0f;

        rlImGuiSetup(true);
        SetupClientNetwork();
    } else {
        StartServer();
    }
}

void Game::Update(float dt) {
    if (!is_server) {
        float scale = fmin(static_cast<float>(GetScreenWidth()) / DESIGN_WIDTH,
                           static_cast<float>(GetScreenHeight()) / DESIGN_HEIGHT);

        camera.offset.x = static_cast<float>(GetScreenWidth()) / 2.0f;
        camera.offset.y = static_cast<float>(GetScreenHeight()) / 2.0f;
        camera.zoom = scale;
    }

    for (const auto &p: players) {
        p.second->Update(dt, players_wish_dirs[p.first]);
    }

    if (is_server) {
        HandleServerNetwork();
    } else {
        HandleClientNetwork();
    }
}

void Game::Render() {
    running = !WindowShouldClose();
    ClearBackground(BLACK);

    BeginMode2D(camera);
    for (const auto &p: players) {
        p.second->Render();
    }

    EndMode2D();
}

void Game::Shutdown() {
    if (!is_server) {
        rlImGuiShutdown();
        CloseWindow();

        ClientDisconnect();
    }

    if (is_server) {
        enet_host_destroy(server);
    }
}
