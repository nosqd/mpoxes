cmake_minimum_required(VERSION 3.29)
project(mpoxes)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    add_compile_options(/utf-8)
endif()

find_package(unofficial-enet CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(tinyfiledialogs CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)

set(
        MPOXES_COMMON_SRC

        Source/Game/Player.cpp
        Source/Game/Player.h
        Source/Game/Packets.h
        Source/Game/Game.h
        Source/Game/Game.cpp

        Source/Math/Vector.h
        Source/Math/Random.h
        Source/Math/Color.h

        Source/Render/Texture.h
        Source/Render/Shader.h
        Source/Render/Mesh.h
        Source/Render/Camera2D.h

        Source/Level/Wall.cpp
        Source/Level/Wall.h
        Source/Level/Level.cpp
        Source/Level/Level.h

        Source/Level/LevelLoader.cpp
        Source/Level/LevelLoader.h
        Source/Math/WorldToScreen.h
)

set(
        MPOXES_SERVER_SRC

        Source/Game/Main.cpp
        Source/Server/GameServer.cpp
)

set(
        MPOXES_CLIENT_SRC

        Source/Client/Input.cpp
        Source/Client/Input.h
        Source/Client/GameDebug.cpp
        Source/Client/GameClient.cpp
        Source/Client/GameData.cpp
        Source/Client/GameData.h
        Source/Game/Main.cpp
)

set(
        MPOXES_EDITOR_SRC
        Source/LevelEditor/LevelEditor.cpp
        Source/LevelEditor/LevelEditor.h
        Source/LevelEditor/Main.cpp
)

add_executable(mpoxes
        ${MPOXES_COMMON_SRC}
        ${MPOXES_CLIENT_SRC}
)
set_target_properties(mpoxes PROPERTIES OUTPUT_NAME "mpoxes")

add_executable(mpoxes-server
        ${MPOXES_COMMON_SRC}
        ${MPOXES_SERVER_SRC}
)
set_target_properties(mpoxes-server PROPERTIES OUTPUT_NAME "mpoxes-server")

add_executable(mpoxes-editor
        ${MPOXES_COMMON_SRC}
        ${MPOXES_EDITOR_SRC}
)
set_target_properties(mpoxes-editor PROPERTIES OUTPUT_NAME "mpoxes-editor")

target_compile_definitions(mpoxes PUBLIC CLIENT)
target_compile_definitions(mpoxes-server PUBLIC SERVER)

add_executable(network-test
        Source/Tests/Packets.Test.cpp
        Source/Game/Packets.h
        Source/Tests/Testing.h

        Source/Level/Level.h
        Source/Level/Wall.h
        Source/Level/Level.cpp
        Source/Level/Wall.cpp
        Source/Tests/Testing.h
)
add_executable(level-test
        Source/Tests/Level.Test.cpp
        Source/Level/Level.h
        Source/Level/Wall.h
        Source/Level/Level.cpp
        Source/Level/Wall.cpp
        Source/Tests/Testing.h
)

set(MPOXES_COMMON_LIBRARIES spdlog::spdlog unofficial::enet::enet glm::glm glad::glad tinyfiledialogs::tinyfiledialogs)
set(MPOXES_CLIENT_LIBRARIES glfw imgui::imgui)
set(MPOXES_SERVER_LIBRARIES)
set(MPOXES_EDITOR_LIBRARIES imguizmo::imguizmo)

target_link_libraries(network-test PRIVATE ${MPOXES_COMMON_LIBRARIES} Catch2::Catch2 Catch2::Catch2WithMain)
target_link_libraries(level-test PRIVATE ${MPOXES_COMMON_LIBRARIES} ${MPOXES_CLIENT_LIBRARIES} Catch2::Catch2 Catch2::Catch2WithMain)

target_link_libraries(mpoxes PRIVATE ${MPOXES_COMMON_LIBRARIES} ${MPOXES_CLIENT_LIBRARIES})
target_link_libraries(mpoxes-server PRIVATE ${MPOXES_COMMON_LIBRARIES} ${MPOXES_SERVER_LIBRARIES})
target_link_libraries(mpoxes-editor PRIVATE ${MPOXES_COMMON_LIBRARIES} ${MPOXES_CLIENT_LIBRARIES} ${MPOXES_EDITOR_LIBRARIES})
target_include_directories(mpoxes PRIVATE ${Stb_INCLUDE_DIR})
