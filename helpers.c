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

void print_help(){
  clear();
  curs_set(0);
  
  attron(COLOR_PAIR(CP_GREEN_BLACK));
  mvaddstr(1, 1,
	    "Instructions for this program:");
  attroff(COLOR_PAIR(CP_GREEN_BLACK));
  mvaddstr(2, 3,
	    "The rubik's cube here is unrolled into 2-D space by a t-shaped");
  mvaddstr(3, 3,
	    "set of squares. The top of the cube is the square in the middle.");
  mvaddstr(4, 3,
	    "Each face is specified with the first letter of its name:");
  mvaddstr(5, 5, "Upper (Top) = \"U\", Down (Bottom) = \"D\", Left = \"L\",");
  mvaddstr(6, 5, "Right = \"R\", Front = \"F\", Back = \"B\",");
  mvaddstr(8, 3,
	   "To turn a slice, such as the middle layer on a 3x3, just specify");
  mvaddstr(9, 3,
	   "the depth before the face's letter, like this: \"2U\"");
  mvaddstr(10, 3,
	   "To turn counter-clockwise, simply add an apostrophe to the end of");
  mvaddstr(11, 3,
	   "the line, like this: \"2U'\"");
  mvaddstr(13, 3,
	   "Pressing enter with no move specified will repeat the most");
  mvaddstr(14, 3,
	   "used move.");
  mvaddstr(16, 3,
	   "'q' quits the program. '?' brings you to this page.");
  mvaddstr(20, 1, "Press any key to continue...");
  
  getch();
  curs_set(2);
}


/**********
 * COLORS *
 **********/

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
