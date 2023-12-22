#include "hash_table.h"
#include <assert.h>
#include <limits.h>
#include <string.h>

int main(){
  ht_hash_table* ht = ht_new();
  ht_insert(ht, "Hello", "World");
  ht_insert(ht, "Its me", "C lang");
  ht_insert(ht, "Steven", "Skiena");
  for(char c=0;c<SCHAR_MAX;++c){
    char* key = malloc(2);
    key[0] = c;
    key[1] = '\0';
    ht_insert(ht, key, "It's me Marian!");
  }

  assert(strcmp(ht_get(ht, "Hello"), "World")==0);
  assert(strcmp(ht_get(ht, "Its me"), "C lang")==0);
  assert(strcmp(ht_get(ht, "Steven"), "Skiena")==0);
  
  ht_del_hash_table(ht);

  return 0;
}
