#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "state.h"
#include "helpers.h"

#define SIDE_LEN 3
#define NUM_FACES 6

struct state_t{
  color **faces;
};

/******************************
 * HELPER FUNCTION PROTOTYPES *
 ******************************/

/* Rotates the given face 90 degrees clockwise.
 */
void rotate_face(color *face);

/* Copies the given side from source over the given side of dest.
 */
void copy_side(color *dest, int dest_side, color *source, int source_side);


/**************************** 
 * FUNCTION IMPLEMENTATIONS *
 ****************************/

state_t *empty_state(){
  state_t *ret = Calloc(1, sizeof(state_t));

  //Allocate our array of sides
  ret->faces = Calloc(NUM_FACES, sizeof(color *));

  //Allocate each side
  for(int i = 0; i < NUM_FACES; i++){
    ret->faces[i] = malloc(SIDE_LEN * SIDE_LEN * sizeof(color));
    memset(ret->faces[i], i, SIDE_LEN * SIDE_LEN);
  }
  
  return ret;
}

void free_state(state_t *s){
  if(s == NULL){
    return;
  }

  if(s->faces != NULL){
    for(int i = 0; i < NUM_FACES; i++){
      free(s->faces[i]);
    }
  }

  free(s->faces);
  free(s);
}

state_t *copy_state(state_t *s){
  state_t *copy = empty_state();

  for(int i = 0; i < NUM_FACES; i++){
    memcpy(copy->faces[i], s->faces[i], SIDE_LEN * SIDE_LEN);
  }

  return copy;
}

state_t *make_move(state_t *s, int face, bool clockwise){
  if(s == NULL || face < 0 || face >= NUM_FACES){
    return NULL;
  }

  //We will be returning this copy
  state_t *copy = copy_state(s);

  //Rotate the side itself
  rotate_face(s->faces[face]);
  if(!clockwise){
    rotate_face(s->faces[face]);
    rotate_face(s->faces[face]);
  }
  
  //Move all the connected sides
  /* It is important to know where on each side the arrays begin. We'll imagine
   * it looks like this:
   *
   *          _______#0                      /------|Side #0
   * #1__   _/  _____#3                   .----.
   *     \_|_|_/_                         |    |----|Side #1
   *     |_|_|_|_|              Side #3|--|    |
   *       |_|\   \__#4                   `----'
   * #5___/    \_____#2                      \------|Side #2
   *
   * And the first element is in the upper left corner of each face in this 
   * image. #2 is also the top of the cube.
   */
  switch(face){
  case 0:
    if(clockwise){
      copy_side(copy->faces[1], 0, s->faces[2], 0);
      copy_side(copy->faces[2], 0, s->faces[3], 0);
      copy_side(copy->faces[3], 0, s->faces[4], 0);
      copy_side(copy->faces[4], 0, s->faces[1], 0);
    }
    else{
      copy_side(copy->faces[1], 0, s->faces[4], 0);
      copy_side(copy->faces[2], 0, s->faces[1], 0);
      copy_side(copy->faces[3], 0, s->faces[2], 0);
      copy_side(copy->faces[4], 0, s->faces[3], 0);
    }
    break;
  case 1:
    if(clockwise){
      copy_side(copy->faces[0], 3, s->faces[4], 1);
      copy_side(copy->faces[2], 3, s->faces[0], 3);
      copy_side(copy->faces[5], 3, s->faces[2], 3);
      copy_side(copy->faces[4], 1, s->faces[5], 3);
    }
    else{
      copy_side(copy->faces[0], 3, s->faces[2], 3);
      copy_side(copy->faces[2], 3, s->faces[5], 3);
      copy_side(copy->faces[5], 3, s->faces[4], 1);
      copy_side(copy->faces[4], 1, s->faces[0], 3);
    }
    break;
  case 2:
    if(clockwise){
      copy_side(copy->faces[0], 2, s->faces[1], 1);
      copy_side(copy->faces[3], 3, s->faces[0], 2);
      copy_side(copy->faces[5], 0, s->faces[3], 3);
      copy_side(copy->faces[1], 1, s->faces[5], 0);
    }
    else{
      copy_side(copy->faces[0], 3, s->faces[3], 3);
      copy_side(copy->faces[3], 2, s->faces[5], 0);
      copy_side(copy->faces[5], 3, s->faces[1], 1);
      copy_side(copy->faces[1], 0, s->faces[0], 2);
    }
    break;
  case 3:
    if(clockwise){
      copy_side(copy->faces[0], 1, s->faces[4], 3);
      copy_side(copy->faces[2], 1, s->faces[0], 1);
      copy_side(copy->faces[5], 1, s->faces[2], 1);
      copy_side(copy->faces[4], 3, s->faces[5], 1);
    }
    else{
      copy_side(copy->faces[0], 1, s->faces[2], 1);
      copy_side(copy->faces[2], 1, s->faces[5], 1);
      copy_side(copy->faces[5], 1, s->faces[4], 3);
      copy_side(copy->faces[4], 3, s->faces[0], 1);
    }
    break;
  case 4:
    if(clockwise){
      copy_side(copy->faces[0], 0, s->faces[3], 1);
      copy_side(copy->faces[3], 1, s->faces[5], 2);
      copy_side(copy->faces[5], 2, s->faces[1], 3);
      copy_side(copy->faces[1], 3, s->faces[0], 0);
    }
    else{
      copy_side(copy->faces[0], 0, s->faces[1], 3);
      copy_side(copy->faces[3], 1, s->faces[0], 0);
      copy_side(copy->faces[5], 2, s->faces[3], 1);
      copy_side(copy->faces[1], 3, s->faces[5], 2);
    }
    break;
  default:
    if(clockwise){
      copy_side(copy->faces[1], 2, s->faces[2], 2);
      copy_side(copy->faces[2], 2, s->faces[3], 2);
      copy_side(copy->faces[3], 2, s->faces[4], 2);
      copy_side(copy->faces[4], 2, s->faces[1], 2);
    }
    else{
      copy_side(copy->faces[1], 2, s->faces[4], 2);
      copy_side(copy->faces[2], 2, s->faces[1], 2);
      copy_side(copy->faces[3], 2, s->faces[2], 2);
      copy_side(copy->faces[4], 2, s->faces[3], 2);
    }
    break;
  }
  
  return copy;
}


/********************
 * HELPER FUNCTIONS *
 ********************/

void rotate_face(color *face){
  if(face == NULL){
    return;
  }

  for(int i = 0; i < SIDE_LEN / 2; i++){
    for (int j = i; j < SIDE_LEN - i - 1; j++){
      int coord = get_coord(j, SIDE_LEN - i - 1, SIDE_LEN);
      int prev_coord = get_coord(i, j, SIDE_LEN);
      
      //Need a temp variable to move in-place
      int temp = face[prev_coord];
 
      //Right to Upper
      face[prev_coord] = face[coord];
      
      //Bottom to Right
      prev_coord = coord;
      coord = get_coord(SIDE_LEN - i - 1, SIDE_LEN - j - 1, SIDE_LEN);
      face[prev_coord] = face[coord];
 
      //Left to Bottom
      prev_coord = coord;
      coord = get_coord(SIDE_LEN - j - 1, i, SIDE_LEN);
      face[prev_coord] = face[coord];
 
      //Temp to Left
      face[coord] = temp;
    }
  }
}

void copy_side(color *dest, int dest_side, color *source, int source_side){
  //Rotate both these sides to the top for easy copying
  if(dest_side != 0){
    for(int i = 0; i + dest_side < 4; i++){
      rotate_face(dest);
    }
  }
  if(source_side != 0){
    for(int i = 0; i + source_side < 4; i++){
      rotate_face(source);
    }
  }

  //Make the copy
  memcpy(dest, source, SIDE_LEN);

  //Rotate them back
  if(dest_side != 0){
    for(int i = 0; i < dest_side; i++){
      rotate_face(dest);
    }
  }
  if(source_side != 0){
    for(int i = 0; i < source_side; i++){
      rotate_face(source);
    }
  }
}
