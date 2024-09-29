#include "Game.h"

#include "../Client/GameData.h"
#include "../Render/Mesh.h"

static void glfw_error_callback(int error, const char *description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void Game::Setup() {
    running = true;
    spdlog::info("Starting mpoxes.");


#ifdef CLIENT
    if (!glfwInit()) {
        spdlog::error("Failed to initialize GLFW3");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

        GameData::Load();

        window = glfwCreateWindow(DESIGN_WIDTH, DESIGN_HEIGHT, "mpoxes", nullptr, nullptr);

        glfwSetErrorCallback(glfw_error_callback);
        glfwMakeContextCurrent(window);
        glfwSwapInterval(0);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            spdlog::error("Failed to initialize GLAD");
            exit(EXIT_FAILURE);
        }

#if defined(__APPLE__)
        const char* glsl_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
        const char *glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        camera = Camera2D(DESIGN_WIDTH, DESIGN_HEIGHT);
        shader = Shader("Shaders/shader.vert", "Shaders/shader.frag");

        SetupClientNetwork();
#else
    StartServer();
#endif
}

void Game::Update(float dt) {
    for (const auto &p: players) {
        p.second->Update(dt, players_wish_dirs[p.first]);
    }

#ifdef SERVER
    HandleServerNetwork();
#endif
#ifdef CLIENT
    HandleClientNetwork();
#endif
}

#ifdef CLIENT
void Game::Render() {
    running = !glfwWindowShouldClose(window);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
    shader.setMat4("projection", camera.getProjectionMatrix());
    shader.setMat4("view", camera.getViewMatrix());
    for (const auto &p: players) {
        p.second->Render(shader);
    }
}
#endif

void Game::Shutdown() {
#ifdef CLIENT
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();

        ClientDisconnect();
        GameData::Save();
#endif
#ifdef SERVER
    enet_host_destroy(server);
#endif
}
