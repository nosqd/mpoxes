//
// Created by null on 9/29/2024.
//

#ifndef COLOR_H
#define COLOR_H
#include "Vector.h"

class Color {
public:
  float r,g,b,a;

  Color(): r(0), g(0), b(0), a(0) {
  }

  Color(float r, float g, float b, float a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
  }

  [[nodiscard]] Vector4 asVec() const {
    return {r,g,b,a};
  }

  bool operator==(const Color& other) const {
    return r == other.r && g == other.g && b == other.b;
  }
};
#endif //COLOR_H
