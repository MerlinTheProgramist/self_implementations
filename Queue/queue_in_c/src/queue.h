#pragma once

#include <stdlib.h>
#include <stdbool.h>

typedef struct{
  int *arr; // not changing
  size_t front; 
  size_t back;
  size_t size;
} Queue;

Queue* new_queue();
void queue_push(Queue *q, int value);
int queue_pop(Queue *q);

size_t queue_len(Queue *q);

int* queue_front(Queue *q);
int* queue_back(Queue *q);

bool queue_is_empty(Queue *q);
bool queue_is_full(Queue *q);

