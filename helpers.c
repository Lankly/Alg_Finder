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

void color_init(){
  start_color();

  //Create colors
  init_color(COLOR_ORANGE, 1000, 269, 0);
  init_color(COLOR_WHITE, 1000, 1000, 1000);

  //Create pairs
  init_pair(CP_BLUE_BLACK,   COLOR_BLUE,   COLOR_BLACK);
  init_pair(CP_GREEN_BLACK,  COLOR_GREEN,  COLOR_BLACK);
  init_pair(CP_ORANGE_BLACK, COLOR_ORANGE, COLOR_BLACK);
  init_pair(CP_RED_BLACK,    COLOR_RED,    COLOR_BLACK);
  init_pair(CP_WHITE_BLACK,  COLOR_WHITE,  COLOR_BLACK);
  init_pair(CP_YELLOW_BLACK, COLOR_YELLOW, COLOR_BLACK);
}
