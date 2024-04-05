#pragma once
#include "point.h"
#include <cmath>
#include <raylib-cpp.hpp>
#include <stdexcept>
#include <iostream>

struct AABB{
  Num x_0, x_1;
  Num y_0, y_1;

  AABB(Num x_0, Num y_0, Num x_1, Num y_1)
  : x_0(x_0)
  , x_1(x_1)
  , y_0(y_0)
  , y_1(y_1)
  {}
  AABB(Point p1, Point p2)
  :AABB(p1.x, p1.y, p2.x, p2.y)
  {}

  bool contains(Point const &p) const{
    // std::cout << p << " against box: [" << x_0<< ',' << y_0 << "][" << x_1 << ',' << y_1 << "]" << std::endl;
    return this->x_0 < p.x && p.x < this->x_1
        && this->y_0 < p.y && p.y < this->y_1;
  }
};

