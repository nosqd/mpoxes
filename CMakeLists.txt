cmake_minimum_required(VERSION 3.29)
project(mpoxes)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(ThirdParty/rlImgui)
find_path(PHYSAC_INCLUDE_DIRS "physac.h")
find_package(unofficial-enet CONFIG REQUIRED)
find_package(raylib CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)

add_executable(mpoxes Source/Game/MPoxes.cpp
        Source/Game/Player.cpp
        Source/Game/Player.h
        Source/Game/Input.cpp
        Source/Game/Input.h
        Source/Game/Packets.h
)

add_executable(network-test Source/Tests/Packets.Test.cpp
        Source/Game/Packets.h)

target_link_libraries(network-test PRIVATE unofficial::enet::enet Catch2::Catch2 Catch2::Catch2WithMain)
target_link_libraries(mpoxes PRIVATE raylib)
target_link_libraries(mpoxes PRIVATE imgui::imgui)
target_link_libraries(mpoxes PRIVATE rlImgui)
target_link_libraries(mpoxes PRIVATE unofficial::enet::enet)
target_include_directories(mpoxes PRIVATE ${PHYSAC_INCLUDE_DIRS})