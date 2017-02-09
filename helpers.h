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

/* Prints a help message to the terminal for the user.
 */
void print_help();

/**********
 * COLORS *
 **********/

#define CP_BLUE_BLACK   1
#define CP_GREEN_BLACK  2
#define CP_ORANGE_BLACK 3
#define CP_RED_BLACK    4
#define CP_WHITE_BLACK  5
#define CP_YELLOW_BLACK 6

#define COLOR_ORANGE 15

void color_init();

#endif
