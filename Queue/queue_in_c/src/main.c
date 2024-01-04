#include "queue.h"
#include <assert.h>
#include <stdio.h>

int main(){
  Queue *q = new_queue(); 
  queue_push(q, 10);
  queue_push(q, 15);
  queue_push(q, 20);
  queue_push(q, 25);

  assert(queue_pop(q) == 10);
  assert(queue_pop(q) == 15);
  
  assert(*queue_back(q) == 20);

  // printf("%zu\n", queue_len(q));
  assert(queue_len(q) == 2);
  queue_push(q, 69);
  assert(*queue_front(q) == 69);
  while(!queue_is_empty(q))
    queue_pop(q);


  for(int i=0;i<50;i++){
    queue_push(q, i);
  }
  assert(queue_is_full(q));
  
  for(int i=0;i<25;i++){
    queue_pop(q);
  }
  assert(queue_len(q) == 25);

  for(int i=0;i<25;i++){
    queue_push(q, i);
  }
  assert(queue_is_full(q));
}
