#ifndef HELPERS_H
#define HELPERS_H

#include<stdlib.h>
#include<curses.h>

//Needed for ncurses
WINDOW *WIN;

//Useful definitions
#define TERMINAL_LENGTH 120
#define TERMINAL_HEIGHT 80

void *Calloc(size_t items, size_t size);
void quit(const char *error_msg);


/* Gets the coordinate in a matrix when that matrix is represented by a
 * one-dimensional array.
 */
int get_coord(int x, int y, int width);

#endif
