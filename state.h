#ifndef STATE_H
#define STATE_H

#include <stdbool.h>

/* Colors are the index of the side they started on.
 */
typedef char color;
typedef struct state_t state_t;

/* Returns a state with every side set to its index.
 */
state_t *new_state(int side_len);

/* Frees a given state.
 */
void free_state(state_t *s);

/* Returns an exact duplicate of a given state.
 */
state_t *copy_state(state_t *s);

/* Returns a new state with the given move on the given state rotated either
 * clockwise or counterclockwise. The move and direction is contained in input.
 */
state_t *make_move(state_t *s, char *input);

/* Returns true if the two given states are the same, including cube 
 * orientation (for example, all sides are solid, but located in a different
 * region of our 2-D mapping returns false when compared with a fresh cube).
 * Both states must be valid.
 */
bool state_equal(state_t *s1, state_t *s2);

/* Prints a 2-D representation of the cube to the terminal, unrolled in a 
 * t-shaped pattern like this:
 *        _
 *      _|_|_ _
 *     |_|_|_|_|
 *       |_|
 */
void print_state(state_t *s);

#endif
