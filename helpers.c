#include <ctype.h>
#include <curses.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "helpers.h"

//Helper Methods

void *Calloc(size_t items, size_t size)
{
  void *ret = calloc(items, size);
  
  if (ret == 0)
  {
    quit("Error: Out of memory!\n");
  }
  
  return ret;
}

void quit(const char *error_msg){
  printf("%s\n",error_msg);
  exit(1);
}

int get_coord(int x, int y, int width){
  return y * width + x;
}
