#include <limits>
#include <cmath>

#include "prime.h"


/*
* Return whether x is prime or not
* 
* Returns: 
*   1 - prime
*   0 - not prime
*  -1 - undefined (i.e. x<2)
*/
size_t is_prime(const size_t x){
  if(x<2) return -1;
  if(x<4) return 1;
  if((x%2) == 0) return 0;
  for(int i=3;i <= floor(std::sqrt(x)); i+=2){
    if(x%i==0)
      return 0;
  }
  return 1;
}


/*
* Returns the next prime after x, or x if x is prime
*/
size_t next_prime(size_t x){
  while(is_prime(x) != 1){
    x++;
  }
  return x;
}