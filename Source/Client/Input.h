#ifndef INPUT_H
#define INPUT_H
#include "../Math/Vector.h"
#include <GLFW/glfw3.h>

namespace Input {
    Vector2 GetMoveDir(GLFWwindow* win);
}

#endif //INPUT_H
