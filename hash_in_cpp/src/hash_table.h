#pragma once
#include <vector>
#include <concepts>
#include "prime.h"


template<typename K, typename V>
class HashTable {
const static size_t INITIAL_BASE_SIZE = 50;
private:
  class Item{
    K key{};
    V value{};
    Item(K& key, V& value):key{key},value{value}
    {}
  };
  size_t base_size{INITIAL_BASE_SIZE};
  size_t size{0};
  size_t count{0};
  std::vector<Item*> items{}; // heap allocated array of pointers


public:
  HashTable()
  : HashTable(INITIAL_BASE_SIZE)
  {}
  HashTable(const size_t base_size)
  : base_size{base_size}
  , size{next_prime(base_size)}
  {
    items = std::vector<Item*>(nullptr, this.size);
  }
  
  ~HashTable(){
    for(Item* item : items)
      delete item;
  }
  void insert(const K& key, const V& value){
    const size_t load = count * 100 / size;
    if(load > 70) // 70%
      resize_up();

    
    Item* curr_item{nullptr};
    size_t index{0}, i{0};
    do{
      // replace if already exists entry with the same key 
      if(curr_item != nullptr && key == curr_item->key){
        items[index] = new Item(key,value);
        return;
      }
      index = get_hash(key, size, i++);
      curr_item = items[index];
    }while(curr_item != nullptr);
    items[index] = new Item(key, value);
    ++count;
  }
  
  V* get(const K& key){
    size_t index = get_hash(key, size, 0);
    Item* curr_item = items[index];
    int i{0};
    while(curr_item!=nullptr){
      if(curr_item->key == key)
        return curr_item->value;

      index = get_hash(key, size, ++i);
      curr_item = items[index];
    }
    return nullptr;
  }
  
  void remove(const K& key){
    const size_t load = count * 100 / size;
    if(load < 10) // < 10%
      resize_down();

    size_t index{get_hash(key, size, 0)};
    Item* item{items[index]};
    size_t i{0};
    while(item!=nullptr){
      if(item->key == key){
        delete item;
        items[index] = nullptr;
      }
      index = get_hash(key, size, ++i);
      item = items[index];
    }
    --count;
  }
private:
  void resize(const size_t base_size){
    if(base_size < INITIAL_BASE_SIZE){
      return;
    }
    std::

    std::vector<Item*> new_vec(nullptr, base_size);
    for(Item* item : this.items)
      if(item!=nullptr)
        new_table.insert(item->key, item->value);

    this.base_size = new_table.base_size;
    this.count = new_table.count;

    std::swap(this.size, new_table.size);
    std::swap(this.items, new_table.items);

    this = new_table;
    
  }
  void resize_up(){
    const size_t new_size = base_size*2;
    resize(new_size);
  }
  void resize_down(){
    const size_t new_size = base_size/2;
    resize(new_size);
  }
};

