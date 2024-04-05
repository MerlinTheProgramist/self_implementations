#pragma once 
#include "consts.h"
#include <Vector2.hpp>
#include <iostream>
#include <math.h>


struct Point;

using Vec2 = Point;

struct Point{
  Num x{},y{};
  Point(Num x, Num y):x(x),y(y)
  {}

  const static Point ZERO;
  
  Num operator[](size_t i)const{
    switch(i){
      case 0: return this->x;
      case 1: return this->y;
      default: throw std::out_of_range("");
    }
  }

  Point operator/(Num const div) const{
    return Point{this->x/div, this->y/div};
  }
  Point operator-(Point const &other)const{
    return Point{this->x-other.x, this->y-other.y};
  }
  Point operator+(Point const &other)const {
    return Point{this->x+other.x, this->y+other.y};
  }
  Point& operator+=(Point const &other){
    this->x+=other.x, this->y+=other.y;
    return *this;
  }
  Point& operator*=(Num const t){
    this->x*=t, this->y*=t;
    return *this;
  }
  Point operator*(Point const &other) const {
    return Point{this->x*other.x, this->y*other.y};
  }
  Point operator*(Num const t) const {
    return Point{this->x*t, this->y*t};
  }
  bool operator==(Point const other) const {
    return this->x==other.x && this->y == other.y;
  }
  bool operator!=(Point const other) const {
    return !(*this==other);
  }

  Num distance_sqr(Point const &other) const{
    return (x-other.x)*(x-other.x) + (y-other.y)*(y-other.y);
  }
  
  inline bool is_zero() const{
    return x==0 && y==0;
  }
  
  Num length_sqr() const{
    return this->x*this->x + this->y*this->y;
  }
  Num length() const{
    return sqrt(this->length_sqr());
  }

  Point normalized() const{
    if(is_zero()) 
      return *this;
    const Num mag = this->length();
    return (*this)/mag;
  }
  void normalize(){
    *this = this->normalized();
  }
    
  // Conversion to raylib's Vector2
  Point(raylib::Vector2 vec2)
  :x(static_cast<Num>(vec2.x))
  ,y(static_cast<Num>(vec2.y))
  {}
  operator Vector2() const{
    return Vector2{static_cast<float>(this->x), static_cast<float>(this->y)};
  }
};

const Point Point::ZERO = Point(0,0);

inline std::ostream& operator<<(std::ostream &os, Point const &p){
  return os <<'('<< p.x<<','<<p.y<<')';
}


