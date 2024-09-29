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
        Source/Game/Input.cpp
        Source/Game/Input.h
        Source/Game/Packets.h
        Source/Game/Game.h
        Source/Game/GameDebug.cpp
        Source/Game/GameClient.cpp
        Source/Game/GameServer.cpp
        Source/Game/Game.cpp
        Source/Game/Vector.h
        Source/Game/Color.h
        Source/Game/Texture.h
        Source/Game/Shader.h
        Source/Game/Mesh.h
        Source/Game/Camera2D.h
        Source/Game/Random.h
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