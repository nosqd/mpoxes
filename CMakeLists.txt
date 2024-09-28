cmake_minimum_required(VERSION 3.29)
project(mpoxes)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(ThirdParty/rlImgui)
find_path(PHYSAC_INCLUDE_DIRS "physac.h")
find_package(unofficial-enet CONFIG REQUIRED)
find_package(raylib CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_executable(mpoxes Source/MPoxes.cpp
        Source/Player.cpp
        Source/Player.h
        Source/Input.cpp
        Source/Input.h
        Source/Packets.h
)

add_executable(enet-test Source/EnetTest.cpp)
add_executable(network-test Source/NetworkTest.cpp
        Source/Packets.h)

target_link_libraries(enet-test PRIVATE unofficial::enet::enet)
target_link_libraries(network-test PRIVATE unofficial::enet::enet)
target_link_libraries(mpoxes PRIVATE raylib)
target_link_libraries(mpoxes PRIVATE imgui::imgui)
target_link_libraries(mpoxes PRIVATE rlImgui)
target_link_libraries(mpoxes PRIVATE unofficial::enet::enet)
target_include_directories(mpoxes PRIVATE ${PHYSAC_INCLUDE_DIRS})