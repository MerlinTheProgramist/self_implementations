#pragma once
#include <stdlib.h>

typedef struct {
  char* key;
  char* value;
} ht_item;

typedef struct {
  size_t base_size;
  size_t size;
  size_t count;
  ht_item** items; // heap allocated array of pointers
} ht_hash_table;

ht_hash_table* ht_new();
ht_hash_table* ht_new_sized(const int base_size);
void ht_del_hash_table(ht_hash_table*);
void ht_insert(ht_hash_table* ht, const char* key, const char* value);
char* ht_get(ht_hash_table* ht, const char* key);
void ht_delete(ht_hash_table* ht, const char* key);