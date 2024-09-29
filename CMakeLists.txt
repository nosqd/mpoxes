cmake_minimum_required(VERSION 3.29)
project(mpoxes)

set(CMAKE_CXX_STANDARD 20)

find_package(unofficial-enet CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(mpoxes Source/Game/Main.cpp
        Source/Game/Player.cpp
        Source/Game/Player.h
        Source/Client/Input.cpp
        Source/Client/Input.h
        Source/Game/Packets.h
        Source/Game/Game.h
        Source/Client/GameDebug.cpp
        Source/Client/GameClient.cpp
        Source/Server/GameServer.cpp
        Source/Game/Game.cpp
        Source/Math/Vector.h
        Source/Math/Color.h
        Source/Render/Texture.h
        Source/Render/Shader.h
        Source/Render/Mesh.h
        Source/Render/Camera2D.h
        Source/Math/Random.h
        Source/Client/GameData.cpp
        Source/Client/GameData.h
)

add_executable(network-test Source/Tests/Packets.Test.cpp
        Source/Game/Packets.h)

target_link_libraries(network-test PRIVATE unofficial::enet::enet Catch2::Catch2 Catch2::Catch2WithMain)
target_link_libraries(mpoxes PRIVATE glfw)
target_link_libraries(mpoxes PRIVATE imgui::imgui)
target_link_libraries(mpoxes PRIVATE spdlog::spdlog)
target_link_libraries(mpoxes PRIVATE glad::glad)
target_link_libraries(mpoxes PRIVATE unofficial::enet::enet)
target_link_libraries(mpoxes PRIVATE glm::glm)
target_include_directories(mpoxes PRIVATE ${Stb_INCLUDE_DIR})