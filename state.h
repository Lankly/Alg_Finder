#ifndef STATE_H
#define STATE_H

#include <stdbool.h>

/* Colors are the index of the side they started on.
 */
typedef char color;
typedef struct state_t state_t;

/* Returns a state with every side set to its index.
 */
state_t *empty_state();

/* Frees a given state.
 */
void free_state(state_t *s);

/* Returns an exact duplicate of a given state.
 */
state_t *copy_state(state_t *s);

/* Returns a new state with the given face on the given state rotated either
 * clockwise or counterclockwise.
 */
state_t *make_move(state_t *s, int face, bool clockwise);

#endif
