#include "hash_table.h"
#include <assert.h>
#include <limits.h>
#include <string.h>
#include <cs.h>

int main(){
  ht_hash_table* ht = ht_new();
  puts("Initialization Success!");
  ht_insert(ht, "Hello", "World");
  ht_insert(ht, "Its me", "C lang");
  ht_insert(ht, "Steven", "Skiena");
  for(int c=0;c<=SCHAR_MAX;++c){
    char* key = malloc(2);
    key[0] = (char)c;
    key[1] = '\0';
    ht_insert(ht, key, "It's me Marian!");
  }
  puts("Insertion Success!");

  assert(strcmp(ht_get(ht, "Hello"), "World")==0);
  assert(strcmp(ht_get(ht, "Its me"), "C lang")==0);
  assert(strcmp(ht_get(ht, "Steven"), "Skiena")==0);
  puts("Feching Success!");

  ht_delete(ht, "Its me");
  assert(ht_get(ht, "Its me") == NULL);
  puts("Deletion Success!");
  
  ht_del_hash_table(ht);
  puts("Destruction Success!");

  return 0;
}
