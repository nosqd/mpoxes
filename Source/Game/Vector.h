#ifndef VECTOR_H
#define VECTOR_H
#include <cmath>
#include <iostream>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Vector2 {
public:
    float x, y;

    // Constructors
    Vector2() : x(0), y(0) {}
    Vector2(float scalar) : x(scalar), y(scalar) {}
    Vector2(float x, float y) : x(x), y(y) {}

    // Vector addition
    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    // Vector subtraction
    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    // Scalar multiplication
    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    // Scalar division
    Vector2 operator/(float scalar) const {
        return Vector2(x / scalar, y / scalar);
    }

    // Dot product
    float dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }

    // Magnitude
    float magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    // Normalization
    Vector2 normalized() const {
        float mag = magnitude();
        return mag > 0 ? *this / mag : Vector2();
    }

    // Print vector
    friend std::ostream& operator<<(std::ostream& os, const Vector2& v) {
        os << "(" << v.x << ", " << v.y << ")";
        return os;
    }

    [[nodiscard]] glm::vec2 as_glm() const {
        return {x, y};
    }
};

class Vector3 {
public:
    float x, y, z;

    // Constructors
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float scalar) : x(scalar), y(scalar), z(scalar) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    // Vector addition
    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    // Vector subtraction
    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    // Scalar multiplication
    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    // Scalar division
    Vector3 operator/(float scalar) const {
        return Vector3(x / scalar, y / scalar, z / scalar);
    }

    // Dot product
    float dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // Cross product
    Vector3 cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    // Magnitude
    float magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    // Normalization
    Vector3 normalized() const {
        float mag = magnitude();
        return mag > 0 ? *this / mag : Vector3();
    }

    // Print vector
    friend std::ostream& operator<<(std::ostream& os, const Vector3& v) {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }

    [[nodiscard]] glm::vec3 as_glm() const {
        return {x, y, z};
    }
};

class Vector4 {
public:
    float x, y, z, w;

    // Constructors
    Vector4() : x(0), y(0), z(0), w(0) {}
    Vector4(float scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    // Vector addition
    Vector4 operator+(const Vector4& other) const {
        return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    // Vector subtraction
    Vector4 operator-(const Vector4& other) const {
        return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    // Scalar multiplication
    Vector4 operator*(float scalar) const {
        return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    // Scalar division
    Vector4 operator/(float scalar) const {
        return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
    }

    // Dot product
    float dot(const Vector4& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    // Magnitude
    float magnitude() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    // Normalization
    Vector4 normalized() const {
        float mag = magnitude();
        return mag > 0 ? *this / mag : Vector4();
    }

    // Print vector
    friend std::ostream& operator<<(std::ostream& os, const Vector4& v) {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
        return os;
    }

    [[nodiscard]] glm::vec4 as_glm() const {
        return {x, y, z, w};
    }
};
#endif //VECTOR_H
