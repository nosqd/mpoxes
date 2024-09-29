//
// Created by null on 9/29/2024.
//

#ifndef COLOR_H
#define COLOR_H

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
};
#endif //COLOR_H
