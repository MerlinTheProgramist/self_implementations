#pragma once
#include "blob.h"
#include "AABB.h"

class Player{
private:  
  std::string name{};
  Num speed{10};

public:
  Point center{0,0};
  std::vector<size_t> blobs_ids{};
  Player(){}
  Player(size_t blob_id)
    :blobs_ids({blob_id})
  {}

};
