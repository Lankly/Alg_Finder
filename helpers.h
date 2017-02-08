#ifndef HELPERS_H
#define HELPERS_H

#include<stdlib.h>

void *Calloc(size_t items, size_t size);
void quit(const char *error_msg);


/* Gets the coordinate in a matrix when that matrix is represented by a
 * one-dimensional array.
 */
int get_coord(int x, int y, int width);

#endif
