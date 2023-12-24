#pragma once
#include <cmath>
#include <cstddef>
#include <string>
#include <vector>
#include <concepts>
#include "prime.h"

// #define LINEAR_PROBING
// #define QUADRATIC_PROBING
#define DOUBLE_HASHING 

template<typename K, typename V>
class HashTable {
  
private:
  const static size_t INITIAL_BASE_SIZE = 53;

  const int HT_PRIME_1 = 50331653;
  const int HT_PRIME_2 = 201326611;

  struct Item{
    K key{};
    V value{};
  };
  Item& DELETED_ITEM{};

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
  , items{size,nullptr}
  , DELETED_ITEM{*new Item{}}
  {}
  
  ~HashTable(){
    for(Item* item : items)
      if(item!=nullptr && item!=&DELETED_ITEM)
        delete item;
  }
  void insert(const K& key, const V& value){
    const size_t load = count * 100 / size;
    if(load > 70) // 70%
      resize_up();

    size_t i{0};
    int index = get_hash(key, size, i);
    Item* curr_item = items[index];
    while(curr_item != nullptr){
      // replace if already exists entry with the same key 
      if(curr_item != &DELETED_ITEM && key == curr_item->key){
        delete items[index];
        items[index] = new Item{key,value};
        return;
      }
      index = get_hash(key, size, i++);
      curr_item = items[index];
    }
    items[index] = new Item{key, value};
    ++count;
  }
  
  V* get(const K& key)const{
    size_t index = get_hash(key, size, 0);
    Item* curr_item = items[index];
    int i{0};
    while(curr_item!=nullptr){
      if(curr_item!=&DELETED_ITEM && curr_item->key == key)
        return &curr_item->value;

      index = get_hash(key, size, ++i);
      curr_item = items[index];
    }
    return nullptr;
  }
  K* operator[](const K& key){
    return this.get(key);
  } 

  void clear(){
    for(Item* item : items){
      if(item!=nullptr && item!=&DELETED_ITEM)
        delete item;
    }
    base_size = INITIAL_BASE_SIZE;
    size = next_prime(base_size);
    items = std::vector<Item*>(size,nullptr);
    count = 0;
  }
  
  void remove(const K& key){
    const size_t load = count * 100 / size;
    if(load < 10) // < 10%
      resize_down();

    size_t index{get_hash(key, size, 0)};
    Item* item{items[index]};
    size_t i{0};
    while(item!=nullptr){
      if(item != &DELETED_ITEM && item->key == key){
        delete item;
        items[index] = &DELETED_ITEM;
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

    // collect all pointer to intems in map
    std::vector<Item*> items_vec{};
    items_vec.reserve(count);

    for(Item* item : this->items)
      if(item!=nullptr && item!=&DELETED_ITEM)
        items_vec.push_back(item);

    this->base_size = base_size;
    this->size = next_prime(base_size);
    this->items = std::vector<Item*>(size,nullptr);
    // count stays the same

    // reinsert all items 
    for(Item* item : items_vec){
      insert(item->key, item->value);
    }
  }
  void resize_up(){
    const size_t new_size = base_size*2;
    resize(new_size);
  }
  void resize_down(){
    const size_t new_size = base_size/2;
    resize(new_size);
  }

  int get_hash(const K& key, const size_t num_buckets, const int attempt) const{
    #ifdef LINEAR_PROBING
    return (hash(s, HT_PRIME_1, num_buckets) + attempt)%num_buckets;  
    #endif

    #ifdef QUADRATIC_PROBING
    return (hash(s, HT_PRIME_1, num_buckets) + (1<<(attempt-1))) % num_buckets; 
    #endif

    #ifdef DOUBLE_HASHING
    const int hash_a = hash(key, HT_PRIME_1, num_buckets);
    const int hash_b = hash(key, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * hash_b + 1)) % num_buckets;
    #endif
  }

  template<typename T>
  int hash(const T& key, const int a, const size_t m) const{
    return 1;
  };

  int hash(const std::basic_string<char>& s, const int a, const size_t m) const{
    long hash = 0;
    const int len_s = s.size();
    for(int i=0;i<len_s;++i){
      hash += std::pow((long)a, (long)((len_s - (i+1)) * s[i]));
      hash %= m;
    }
    return (int)hash;
  }
};

