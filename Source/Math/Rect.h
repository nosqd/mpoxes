#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "Vector.h"

class Rect {
public:
    Vector2 position;
    Vector2 size;

    Rect(Vector2 pos, Vector2 s) : position(pos), size(s) {}

    bool Contains(const Vector2& point) const {
        return point.x >= position.x && point.x <= position.x + size.x &&
               point.y >= position.y && point.y <= position.y + size.y;
    }

    bool Intersects(const Rect& other) const {
        return !(other.position.x > position.x + size.x ||
                 other.position.x + other.size.x < position.x ||
                 other.position.y > position.y + size.y ||
                 other.position.y + other.size.y < position.y);
    }
};
#endif //RECTANGLE_H
