#include <chrono>
#include <cstring>
#include "Game.h"

int main(int argc, char *argv[]) {
    Game game{};

    if (argc > 1 && std::strcmp(argv[1], "server") == 0) {
        game.is_server = true;
    } else {
        game.is_server = false;
    }

    if (enet_initialize() != 0) {
        spdlog::error("An error occurred while initializing ENet");
        return EXIT_FAILURE;
    }

    game.Setup();

    std::chrono::steady_clock::time_point lastFrameTime = std::chrono::steady_clock::now();

    while (game.running) {
        auto currentTime = std::chrono::steady_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
        lastFrameTime = currentTime;

        game.Update(deltaTime);

        if (!game.is_server) {
            glfwPollEvents();

            game.Render();
            game.RenderImGui(deltaTime);

            glfwSwapBuffers(game.window);
        }
    }

    game.Shutdown();
    enet_deinitialize();
    return 0;
}
