#pragma once
#include <algorithm>
#include <cstddef>
#include <memory>
#include <raylib-cpp.hpp>
#include <stdexcept>
#include "AABB.h"

struct Quad{
  Point center;
  Point radius;
  Quad operator[](size_t i) const{
    auto half_radius = this->radius/2.0;
    switch(i){
      case 0: return Quad{{center+half_radius*Point{-1,-1}},{half_radius}};
      case 1: return Quad{{center+half_radius*Point{-1, 1}},{half_radius}};
      case 2: return Quad{{center+half_radius*Point{ 1,-1}},{half_radius}};
      case 3: return Quad{{center+half_radius*Point{ 1, 1}},{half_radius}};
      default: throw std::range_error("Range of Quad is in [0,3]");
    }
  }
  inline size_t getSubQuadIndex(Point const &pos) const{
    if(pos.x < center.x){
      if(pos.y < center.y)
        return 0; // -x, -y
      return 1;   // -x, +y 
    }

    if(pos.y < center.y)
      return 2; // +x, -y
    return 3;   // +x, +y
  }
};

bool circleRect_intersect(Point const &c, Num radius, Quad const &q) {

  // temporary variables to set edges for testing
  Num testX = c.x;
  Num testY = c.y;

  
  // which edge is closest?
  if (c.x < q.center.x-q.radius.x)         testX = q.center.x-q.radius.x;      // test left edge
  else if (c.x > q.center.x+q.radius.x) testX = q.center.x+q.radius.x;   // right edge
  if (c.y < q.center.y-q.radius.y)         testY = q.center.y-q.radius.y;      // top edge
  else if (c.y > q.center.y+q.radius.y) testY = q.center.y+q.radius.y;   // bottom edge

  // get distance from closest edges
  Num distX = c.x-testX;
  Num distY = c.y-testY;
  Num distance = sqrt( (distX*distX) + (distY*distY) );

  // if the distance is less than the radius, collision!
  if (distance <= radius) {
    return true;
  }
  return false;
}



template<typename T, size_t CAPACITY, size_t HALF_WIDTH, size_t HALF_HEIGHT>
class QuadTree{
  using This = QuadTree<T, CAPACITY, HALF_WIDTH, HALF_HEIGHT>;
  private:

  struct Element{
    T data;
    Point pos;
  };
  std::vector<Element> elements{};

  std::vector<std::unique_ptr<This>> subTrees{};

  #define MY_QUAD Quad{Point{HALF_WIDTH/2.,HALF_HEIGHT/2.}, Point{HALF_WIDTH/2., HALF_HEIGHT/2.}}
  
  private:
  bool divided() const{
    return !subTrees.empty();
  }
  
  public:
  QuadTree(){
    elements.reserve(CAPACITY);
  }
  Quad insert(Point const &pos, T const &obj, Quad const quad=MY_QUAD){
    if(!this->divided()){
      if(this->elements.size()<CAPACITY){
        this->elements.push_back(Element{obj, pos});
        return quad;
      }
      this->split(quad); // split and insert to proper child
    }

    // insert to child
    auto const quad_id = quad.getSubQuadIndex(pos);
    return this->subTrees[quad_id]->insert(pos, obj, quad[quad_id]);
  }
  bool remove(Point const &pos, Quad const q = MY_QUAD){

    if(this->divided()){
      auto const quad_id = q.getSubQuadIndex(pos);
      return subTrees[quad_id]->remove(pos, q[quad_id]);
    }

    auto it = std::find_if(this->elements.begin(), this->elements.end(), [&](Element const &el){return el.pos==pos;});
    if(it == this->elements.end())
      return false;

    this->elements.erase(it);
    return true;
  }

  using Filter = bool(const Point&, const T&);
  static bool defaultFilter(const Point&, const T&){return true;}
  
  template<Filter filter=defaultFilter>
  void query_rect(AABB const &bbox, std::vector<T> &selected, Quad const q = MY_QUAD) const {
    if(bbox.x_1 < q.center.x-q.radius.x
    || q.center.x+q.radius.x < bbox.x_0
    || bbox.y_1 < q.center.y-q.radius.y
    || q.center.y+q.radius.y < bbox.y_0)
      return;

    if(this->divided()){
      for(int i=0;i<4;++i){
        subTrees[i]->template query_rect<filter>(bbox, selected, q[i]);
      }
      return;
    }

    for(int i=0;i<this->elements.size();++i){
      if(bbox.contains(this->elements[i].pos) && filter(this->elements[i].pos, this->elements[i].data))
        selected.push_back(this->elements[i].data);
    }
  }

  template<Filter filter=defaultFilter>
  void query_circle(Vec2 const &center, Num const r, std::vector<T> &selected, Quad const q=MY_QUAD)const{
    if(!circleRect_intersect(center, r, q))
      return;

    if(this->divided()){
      for(int i=0;i<4;++i)
        subTrees[i]->template query_circle<filter>(center, r, selected, q[i]);
      return;
    }

    for(int i=0;i<this->elements.size();++i){
      if(filter(this->elements[i].pos, this->elements[i].data))
        selected.push_back(this->elements[i].data);
    }
  }
  
  private:
  void split(Quad const &quad){
    this->subTrees.reserve(4); // Quad = 4
    for(int i{0};i<4;++i){
      this->subTrees.emplace_back(std::make_unique<This>());
    }

    // reallocate objects to child nodes
    for(size_t i=0;i<this->elements.size();++i){
      auto const &obj = this->elements[i].data;
      auto const &pos = this->elements[i].pos;
      
      auto quad_id = quad.getSubQuadIndex(pos);
      this->subTrees[quad_id]->insert(pos, obj ,quad[quad_id]);
    }

    this->elements.clear();
    this->elements.shrink_to_fit();
  }

};
