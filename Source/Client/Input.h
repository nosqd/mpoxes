#ifndef INPUT_H
#define INPUT_H
#include "Vector.h"
#include <../../cmake-build-debug/vcpkg_installed/x64-windows/include/GLFW/glfw3.h>

namespace Input {
    Vector2 GetMoveDir(GLFWwindow* win);
}

#endif //INPUT_H
