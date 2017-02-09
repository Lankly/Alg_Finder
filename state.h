#ifndef STATE_H
#define STATE_H

#include <stdbool.h>

/* Colors are the index of the side they started on.
 */
typedef char color;
typedef struct state_t state_t;

state_t *empty_state();
void free_state(state_t *s);
state_t *copy_state(state_t *s);
state_t *make_move(state_t *s, int face, bool clockwise);

#endif
