#include <stdbool.h>
#include <stdlib.h>

#include "queue.h"

const size_t QUEUE_INITIAL_SIZE = 50;


Queue* new_queue_sized(size_t size){
  Queue *q = malloc(sizeof(Queue));
  q->size = QUEUE_INITIAL_SIZE;
  q->arr = malloc(QUEUE_INITIAL_SIZE);
  q->back =  0;
  q->front = -1;
  return q;
}

Queue* new_queue(){
  return new_queue_sized(QUEUE_INITIAL_SIZE);
}

size_t queue_len(Queue *q){
  // return (q->back - q->arr)-(q->front - q->arr); // because of wrapping
  int diff = q->front - q->back;
  if(diff < 0){
    return q->size + diff + 1;
  }else{
    return diff + 1;
  }
  
}

bool queue_is_empty(Queue *q){
  return q->front == q->back-1 || (q->front == q->back-1+q->size);
}

bool queue_is_full(Queue *q){
   return queue_len(q) == q->size;
}

void queue_push(Queue *q, int value){
  q->front++;
  if(q->front > q->size)
    q->front -= q->size;
  q->arr[q->front] = value;
}
int queue_pop(Queue *q){
  int val = q->arr[q->back];
  q->back++;
  if(q->back > q->size)
    q->back -= q->size;
  return val;
}

int* queue_front(Queue *q){
  return &q->arr[q->front];
}
int* queue_back(Queue *q){
  return &q->arr[q->back];
}
