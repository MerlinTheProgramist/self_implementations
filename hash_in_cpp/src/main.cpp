#include "hash_table.h"
#include <cassert>
#include <limits>
#include <string>

int main(){
  HashTable<std::string, std::string> ht{};
  ht.insert("Hello", "World");
  ht.insert("Its me", "C lang");
  ht.insert("Steven", "Skiena");
  for(char c=0;c<std::numeric_limits<char>::max();++c){
    ht.insert(std::string{c}, "It's me Marian!");
  }

  assert(*ht.get("Hello")== "World");
  assert(*ht.get("Its me")== "C lang");
  assert(*ht.get("Steven")== "Skiena");

  ht.remove("Hello");
  assert(ht.get("Hello") == nullptr);
  ht.remove("Its me");
  assert(ht.get("Its me") == nullptr);
  ht.remove("Steven");
  assert(ht.get("Steven") == nullptr);

  

  return 0;
}
