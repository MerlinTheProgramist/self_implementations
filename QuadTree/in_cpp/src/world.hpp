#include "blob.h"
#include "quadTree.h"

template<size_t WORLD_WIDTH, size_t WORLD_HEIGHT>
class World{
  std::vector<Blob> all_blobs{};
  std::vector<size_t> dynamic{};
  QuadTree<size_t, 5, WORLD_WIDTH, WORLD_HEIGHT> quadTree{};
  
  World()
  {}

  void new_blob(
    Point const &pos, 
    size_t size = 3, 
    Color const &colour = GREEN, 
    Point const &vel = {0,0}){
      all_blobs.emplace_back(pos, size, colour, vel);
      size_t id = all_blobs.size()-1;
      if(vel!=Point{0,0}){
        dynamic.push_back(id);

      }
    }
};
