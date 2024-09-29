# MPOxes
> MultiPlayer bOxes

Simple c++ 2d multiplayer game built with opengl and enet where you can move player as a box.

## Why?

C++ always was my enemy, sometime ago I developed launcher for my game and it crashed after some time with HEAP_CORRUPTION 
error, no logs. And networking was my enemy. And gamedev....

## How to build?

I'm builtin CLion vcpkg support, but you can just integrate vcpkg in your cmake and build project by your own.

## Dependencies

- imgui (vcpkg) - Bloat-free Graphical User interface for C++ with minimal dependencies
- enet (vcpkg) - Reliable UDP networking library
- catch2 (vcpkg) - A modern, C++-native, test framework for unit-tests, TDD and BDD
- glfw3 (vcpkg) - Open Source, multi-platform library for OpenGL, OpenGL ES and Vulkan development on the desktop
- spdlog (vcpkg) - Fast C++ logging library
- glad (vcpkg) - Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator based on the official specs.
- stb (vcpkg) - stb single-file public domain libraries for C/C++
- glm (vcpkg) - OpenGL Mathematics (GLM)
- nlohmann-json (vcpkg) - JSON for Modern C++
- tinyfiledialogs (vcpkg) - Highly portable and cross-platform dialogs for native inputbox, passwordbox, colorpicker and more
- imguizmo (vcpkg) - Immediate mode 3D gizmo for scene editing and other controls based on Dear Imgui

## How to use.

### Server
```shell
$ ./mpoxes-server
``` 
spins up a brand-new server open 7777 port

### Client
```shell
$ ./mpoxes
```
starts client