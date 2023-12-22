#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"
#include "prime.h"

// #define LINEAR_PROBING
// #define QUADRATIC_PROBING
#define DOUBLE_HASHING 


// Constants
static size_t HT_INITIAL_BASE_SIZE = 53;
static ht_item HT_DELETED_ITEM = {NULL, NULL};
const int HT_PRIME_1 = 50331653;
const int HT_PRIME_2 = 201326611;


static void ht_resize(ht_hash_table* ht, const int base_size){
  if(base_size < HT_INITIAL_BASE_SIZE){
    return;
  }
  ht_hash_table* new_ht = ht_new_sized(base_size);
  for(int i=0;i<ht->size;++i){
    ht_item* item = ht->items[i];
    if(item != NULL && item != &HT_DELETED_ITEM)
      ht_insert(new_ht, item->key, item->value);
  }

  ht->base_size = new_ht->base_size;
  ht->count = new_ht->count;

  const int tmp_size = ht->size;
  ht->size = new_ht->size;
  new_ht->size = tmp_size;

  ht_item** tmp_items = ht->items;
  ht->items = new_ht->items;
  new_ht->items = tmp_items;

  ht_del_hash_table(new_ht);
}


static void ht_del_item(ht_item* i){
  free(i->key);
  free(i->value);
  free(i);
}

void ht_del_hash_table(ht_hash_table* ht){
  for(int i=0;i<ht->size;++i){
    ht_item* item = ht->items[i];
    if(item!=NULL){
      ht_del_item(item);
    }
  }
  free(ht->items);
  free(ht);
}

static int ht_hash(const char* s, const int a, const int m){
  long hash = 0;
  const int len_s = strlen(s);
  for(int i=0;i<len_s;++i){
    hash += (long)pow(a, len_s - (i+1)) * s[i];
    hash %= m;
  }
  return (int)hash;
}

// Double Hashing (method of handling collisions)
static int ht_get_hash(const char* s, const int num_buckets, const int attempt){
  #ifdef LINEAR_PROBING
  return (ht_hash(s, HT_PRIME_1, num_buckets) + attempt)%num_buckets;  
  #endif

  #ifdef QUADRATIC_PROBING
  return (ht_hash(s, HT_PRIME_1, num_buckets) + (1<<(attempt-1))) % num_buckets; 
  #endif

  #ifdef DOUBLE_HASHING
  const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
  const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
  return (hash_a + (attempt * hash_b + 1)) % num_buckets;
  #endif
}




