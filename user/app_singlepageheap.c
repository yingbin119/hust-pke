/*
 * Below is the given application for lab2_challenge2_singlepageheap.
 * This app performs malloc memory.
 */

#include "user_lib.h"
#include "util/string.h"

typedef unsigned long long uint64;

int main(void) {
  
  char str[20] = "hello, world!!!";
  char *m = (char *)malloc(100);
  char *p = (char *)malloc(50);
  if((uint64)p - (uint64)m > 512 ){
    printu("you need to manage the vm space precisely!");
    exit(-1);
  }
  free((void *)m);

  strcpy(p,str);
  printu("%s\n",p);
  char *n = (char *)malloc(50);

  if(m != n)
  {
    printu("your malloc is not complete.\n");
    exit(-1);
  }
  exit(0);
  return 0;
}
