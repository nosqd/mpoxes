# MPOxes
> MultiPlayer bOxes

Simple c++ 2d multiplayer game built with raylib and enet where you can move player as a box.

## Why?

C++ always was my enemy, sometime ago I developed launcher for my game and it crashed after some time with HEAP_CORRUPTION 
error, no logs. And networking was my enemy. And gamedev....

## How to build?

I'm builtin CLion vcpkg support, but you can just integrate vcpkg in your cmake and build project by your own.

## Dependencies

- raylib (vcpkg) - Simple and easy-to-use library to enjoy videogames programming
- imgui (vcpkg) - Bloat-free Graphical User interface for C++ with minimal dependencies
- enet (vcpkg) - Reliable UDP networking library
- physac (vcpkg) - (unused now)  2D physics header-only library for videogames developed in C using raylib library.
- rlImgui ([ThirdParty/rlImgui/](ThirdParty/rlImgui/)) - Raylib integration with DearImGui. (backported to imgui v1.91.0 by me)

## How to use.

### Server
```shell
$ ./mpoxes server
``` 
spins up a brand-new server open 7777 port

### Client
```shell
$ ./mpoxes client
```
starts client that will connect to 127.0.0.1:7777