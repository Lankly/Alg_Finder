#include "helpers.h"
#include <ctype.h>
#include <curses.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

//Helper Methods

void *Calloc(size_t items, size_t size)
{
  void *ret = calloc(items, size);
  
  if (ret == 0)
  {
    quit("Error: out of memory!\n");
  }
  
  return ret;
}

void quit(const char *error_msg){
  endwin();
  printf("%s\n",error_msg);
  exit(1);
}
