#include <chrono>
#include <cstring>
#include "Game.h"

#ifdef CLIENT
#pragma message ("Building mpoxes client")
#elif defined(SERVER)
#pragma message ("Building mpoxes server")
#else
#error Unsupported game mode
#endif

int main(int argc, char *argv[]) {
    Game game{};

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

        #ifdef CLIENT
            glfwPollEvents();

            game.Render();
            game.RenderImGui(deltaTime);

            glfwSwapBuffers(game.window);
        #endif
    }

    game.Shutdown();
    enet_deinitialize();
    return 0;
}
