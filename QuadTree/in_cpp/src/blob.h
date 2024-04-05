#pragma once
#include <algorithm>
#include <cmath>
#include <iostream>
#include "AABB.h"
#include "consts.h"
class Blob{
  public:
  Point pos{0,0};
  Num radius{};
  Vec2 velocity{0,0};
  Color color{GREEN};

  const static Num DEFAULT_R;
    
  Blob(Point pos, Num r=DEFAULT_R, Color color=GREEN, Point vel={0,0})
   :pos(pos)
   ,radius(r)
   ,velocity(vel)
   ,color(color)
  {}

  /// returns true if still moving
  bool update(float dt){
    this->pos += velocity*dt;
    velocity *= 1.0 - (0.9 * dt);
    
    if(velocity.length_sqr() > 0.1)
      return true;

    velocity = Point::ZERO;
    return false;
  }

  Num area() const{
    return radius*radius*PI;
  }

  private:
    static inline Num area_to_radius(Num area){
      return std::sqrt(area/PI);
    }
    
  public:
  void merge_size(Blob const &other, bool sign=true){
    Num area_diff = other.area()*((sign)?1:-1);
    this->radius = area_to_radius(this->area()+area_diff);
  }

  /// Takes normalized dir
  Blob shut_mini_out(Vec2 const &dir){
    auto junior = Blob(
      this->pos + dir*this->radius, 
      DEFAULT_R, 
      this->color, 
      dir*MINI_BLOB_SHOT_SPEED);
    this->radius = area_to_radius(this->area() - junior.area());
    return junior;
  }
  
  Blob split(Vec2 const &normal={0,0}){
    const Num r = this->radius;
    const Num half_r = r/2.0;
    
    this->radius = half_r;
    this->velocity += normal*(-BLOB_SPLIT_VELOCITY_MULTIPLIER*radius);
    
    return Blob{
      this->pos+normal*half_r, 
      half_r,
      this->color,
      normal*BLOB_SPLIT_VELOCITY_MULTIPLIER*radius,
    };
  }

  /// returns true if other is in bounds of this
  bool test_against(Blob const &other) const{
    return radius > other.radius && pos.distance_sqr(other.pos) < (radius-other.radius)*(radius-other.radius);
  }
};

const Num Blob::DEFAULT_R = 3;
