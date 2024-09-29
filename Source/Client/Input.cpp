#include "Input.h"
#include <cmath>
#include <../../cmake-build-debug/vcpkg_installed/x64-windows/include/GLFW/glfw3.h>

Vector2 Input::GetMoveDir(GLFWwindow* win) {
    Vector2 wish_dir = {0.0f, 0.0f};

    if (glfwGetKey(win, GLFW_KEY_W)) wish_dir.y += 1.0f;
    if (glfwGetKey(win, GLFW_KEY_S)) wish_dir.y -= 1.0f;
    if (glfwGetKey(win, GLFW_KEY_A)) wish_dir.x -= 1.0f;
    if (glfwGetKey(win, GLFW_KEY_D)) wish_dir.x += 1.0f;

    // Normalize the vector if it's not zero
    if (wish_dir.x != 0.0f || wish_dir.y != 0.0f) {
        float length = sqrtf(wish_dir.x * wish_dir.x + wish_dir.y * wish_dir.y);
        wish_dir.x /= length;
        wish_dir.y /= length;
    }

    return wish_dir;
}
