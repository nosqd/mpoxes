cmake_minimum_required(VERSION 3.10)
project(rlImgui VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find required packages
find_package(raylib REQUIRED)
find_package(imgui REQUIRED)

# Add library
add_library(rlImgui STATIC rlImgui.cpp)

# Link libraries
target_link_libraries(rlImgui PUBLIC raylib imgui::imgui)

# Include directories
target_include_directories(rlImgui PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)