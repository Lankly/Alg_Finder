#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <curses.h>
#include "state.h"
#include "helpers.h"

#define NUM_FACES 6

struct state_t{
  int side_len;
  color **faces;
};


/* It is important to know where on each side of a state the arrays begin.
 * We'll imagine the cube looks like this:
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

/******************************
 * HELPER FUNCTION PROTOTYPES *
 ******************************/

/* Rotates the given face 90 degrees clockwise.
 */
void rotate_face(color *face, int side_len);

/* Copies the given side from source over the given side of dest. Depth is used
 * to determine which whether the outermost layer should be used, or a layer 
 * closer to the center.
 */
void copy_side(color *dest,
               int dest_side,
               color *source,
               int source_side,
               int side_len,
	       int depth);

/* Returns the first letter of the given color.
 */
int ctoa(color c);

/* Returns the index of the face represented by str, or -1 if it did not find
 * anything. For example, if given the string "U", 2 would be returned, since
 * that's the index of the upper face. str must be a valid, NULL-Terminated
 * string.
 */
int get_face(char *str);

/* Returns the depth that the turn should happen on. For example, given the
 * input "2U" on a 3x3 cube, the depth would be 1, representing the middle
 * slice of the cube to turn. Returns 0 by default. str must be a valid, 
 * NULL-Terminated string.
 */
int get_depth(char *str);

/* Returns true if the command does not end in an apostrophe, indicating that
 * the move should be clockwise. str must be a valid, NULL-Terminated string.
 */
bool is_clockwise(char *str);

/**************************** 
 * FUNCTION IMPLEMENTATIONS *
 ****************************/

state_t *new_state(int side_len){
  //If side_len is invalid, default to 3
  if(side_len < 0){
    side_len = 3;
  }
  
  state_t *ret = Calloc(1, sizeof(state_t));
  ret->side_len = side_len;
  
  //Allocate our array of sides
  ret->faces = Calloc(NUM_FACES, sizeof(color *));

  //Allocate each side
  for(int i = 0; i < NUM_FACES; i++){
    ret->faces[i] = malloc(side_len * side_len * sizeof(color));
    if(ret->faces[i] == NULL){
      quit("Error! Out of memory!\n");
    }
    
    memset(ret->faces[i], i, side_len * side_len);
  }

  return ret;
}

void free_state(state_t *s){
  if(s == NULL){
    return;
  }

  free(s->faces);
  free(s);
}

state_t *copy_state(state_t *s){
  state_t *copy = new_state(s->side_len);

  for(int i = 0; i < NUM_FACES; i++){
    memcpy(copy->faces[i], s->faces[i], s->side_len * s->side_len);
  }

  return copy;
}

state_t *make_move(state_t *s, char *input){
  //We will be returning this copy
  state_t *copy = copy_state(s);
  
  //Stop if s is uninitialized or face is out of bounds
  if(s == NULL){
    return copy;
  }

  //Parse the input
  int face = get_face(input);
  int depth = get_depth(input);
  bool clockwise = is_clockwise(input);

  //Stop if the face was invalid
  if(face < 0){
    return copy;
  }
  
  //Rotate the side itself (don't do this if turning an interior slice)
  if(depth == 0){
    rotate_face(copy->faces[face], copy->side_len);
    if(!clockwise){
      rotate_face(copy->faces[face], copy->side_len);
      rotate_face(copy->faces[face], copy->side_len);
    }
  }
  
  //Move all the connected sides
  switch(face){
  case 0:
    if(clockwise){
      copy_side(copy->faces[1], 0, s->faces[2], 0, s->side_len, depth);
      copy_side(copy->faces[2], 0, s->faces[3], 0, s->side_len, depth);
      copy_side(copy->faces[3], 0, s->faces[4], 0, s->side_len, depth);
      copy_side(copy->faces[4], 0, s->faces[1], 0, s->side_len, depth);
    }
    else{
      copy_side(copy->faces[1], 0, s->faces[4], 0, s->side_len, depth);
      copy_side(copy->faces[2], 0, s->faces[1], 0, s->side_len, depth);
      copy_side(copy->faces[3], 0, s->faces[2], 0, s->side_len, depth);
      copy_side(copy->faces[4], 0, s->faces[3], 0, s->side_len, depth);
    }
    break;
  case 1:
    if(clockwise){
      copy_side(copy->faces[0], 3, s->faces[4], 1, s->side_len, depth);
      copy_side(copy->faces[2], 3, s->faces[0], 3, s->side_len, depth);
      copy_side(copy->faces[5], 3, s->faces[2], 3, s->side_len, depth);
      copy_side(copy->faces[4], 1, s->faces[5], 3, s->side_len, depth);
    }
    else{
      copy_side(copy->faces[0], 3, s->faces[2], 3, s->side_len, depth);
      copy_side(copy->faces[2], 3, s->faces[5], 3, s->side_len, depth);
      copy_side(copy->faces[5], 3, s->faces[4], 1, s->side_len, depth);
      copy_side(copy->faces[4], 1, s->faces[0], 3, s->side_len, depth);
    }
    break;
  case 2:
    if(clockwise){
      copy_side(copy->faces[0], 2, s->faces[1], 1, s->side_len, depth);
      copy_side(copy->faces[3], 3, s->faces[0], 2, s->side_len, depth);
      copy_side(copy->faces[5], 0, s->faces[3], 3, s->side_len, depth);
      copy_side(copy->faces[1], 1, s->faces[5], 0, s->side_len, depth);
    }
    else{
      copy_side(copy->faces[0], 2, s->faces[3], 3, s->side_len, depth);
      copy_side(copy->faces[3], 3, s->faces[5], 0, s->side_len, depth);
      copy_side(copy->faces[5], 0, s->faces[1], 1, s->side_len, depth);
      copy_side(copy->faces[1], 1, s->faces[0], 2, s->side_len, depth);
    }
    break;
  case 3:
    if(clockwise){
      copy_side(copy->faces[0], 1, s->faces[4], 3, s->side_len, depth);
      copy_side(copy->faces[2], 1, s->faces[0], 1, s->side_len, depth);
      copy_side(copy->faces[5], 1, s->faces[2], 1, s->side_len, depth);
      copy_side(copy->faces[4], 3, s->faces[5], 1, s->side_len, depth);
    }
    else{
      copy_side(copy->faces[0], 1, s->faces[2], 1, s->side_len, depth);
      copy_side(copy->faces[2], 1, s->faces[5], 1, s->side_len, depth);
      copy_side(copy->faces[5], 1, s->faces[4], 3, s->side_len, depth);
      copy_side(copy->faces[4], 3, s->faces[0], 1, s->side_len, depth);
    }
    break;
  case 4:
    if(clockwise){
      copy_side(copy->faces[0], 0, s->faces[3], 1, s->side_len, depth);
      copy_side(copy->faces[3], 1, s->faces[5], 2, s->side_len, depth);
      copy_side(copy->faces[5], 2, s->faces[1], 3, s->side_len, depth);
      copy_side(copy->faces[1], 3, s->faces[0], 0, s->side_len, depth);
    }
    else{
      copy_side(copy->faces[0], 0, s->faces[1], 3, s->side_len, depth);
      copy_side(copy->faces[3], 1, s->faces[0], 0, s->side_len, depth);
      copy_side(copy->faces[5], 2, s->faces[3], 1, s->side_len, depth);
      copy_side(copy->faces[1], 3, s->faces[5], 2, s->side_len, depth);
    }
    break;
  default:
    if(clockwise){
      copy_side(copy->faces[1], 2, s->faces[2], 2, s->side_len, depth);
      copy_side(copy->faces[2], 2, s->faces[3], 2, s->side_len, depth);
      copy_side(copy->faces[3], 2, s->faces[4], 2, s->side_len, depth);
      copy_side(copy->faces[4], 2, s->faces[1], 2, s->side_len, depth);
    }
    else{
      copy_side(copy->faces[1], 2, s->faces[4], 2, s->side_len, depth);
      copy_side(copy->faces[2], 2, s->faces[1], 2, s->side_len, depth);
      copy_side(copy->faces[3], 2, s->faces[2], 2, s->side_len, depth);
      copy_side(copy->faces[4], 2, s->faces[3], 2, s->side_len, depth);
    }
    break;
  }
  
  return copy;
}

void print_state(state_t *s){
  if(s == NULL || s->faces == NULL){
    return;
  }

  int max_line_len = (NUM_FACES - 2) * s->side_len + (NUM_FACES - 2) + 1;
  int current_line = 0;
  
  /* Print #0 first */
  // First part is the top of the surrounding box
  move(0, s->side_len + 1);
  addch(ACS_ULCORNER);
  for(int i = 0; i < s->side_len; i++){
    addch(ACS_HLINE);
  }
  addch(ACS_URCORNER);
  current_line++;

  for(int i = 0; i < s->side_len; i++){
    //Second part is the whitespace, following by the side of the box
    move(current_line, s->side_len + 1);
    addch(ACS_VLINE);
    
    for(int j = 0; j < s->side_len; j++){
      //We need to convert the color to something we can actually represent
      color c = s->faces[0][get_coord(j, i, s->side_len)];
      int actual_color = ctoa(c);

      //Then we can add it to the buffer
      addch(actual_color);
    }

    //Finally add the other side of the box
    addch(ACS_VLINE);
    
    current_line++;
  }

  /* Print the top of the long row of faces */
  move(current_line, 0);
  addch(ACS_ULCORNER);
  for(int i = 0; i < max_line_len - 2; i++){
    addch(ACS_HLINE);
  }
  mvaddch(current_line, 1 + s->side_len, ACS_PLUS);
  mvaddch(current_line, 2 * (1 + s->side_len), ACS_PLUS);
  mvaddch(current_line, 3 * (1 + s->side_len), ACS_TTEE);
  mvaddch(current_line, max_line_len - 1, ACS_URCORNER);
  current_line++;
  
  /* Print #1 through #4 next, all in a line */
  for(int i = 0; i < s->side_len; i++){
    move(current_line, 0);

    for(int j = 0; j < s->side_len * (NUM_FACES - 2); j++){
      
      //Print the side of a box whenever we reach the start of a new face
      if(j % s->side_len == 0){
        addch(ACS_VLINE);
      }

      int coord = get_coord(j % s->side_len, i, s->side_len);
      color c = s->faces[1 + j / s->side_len][coord];
      int actual_color = ctoa(c);

      addch(actual_color);
    }
    addch(ACS_VLINE);
    current_line++;
  }

  /* Print the bottom of the long row of faces */
  move(current_line, 0);
  addch(ACS_LLCORNER);
  for(int i = 0; i < max_line_len - 2; i++){
    addch(ACS_HLINE);
  }
  mvaddch(current_line, 1 + s->side_len, ACS_PLUS);
  mvaddch(current_line, 2 * (1 + s->side_len), ACS_PLUS);
  mvaddch(current_line, 3 * (1 + s->side_len), ACS_BTEE);
  mvaddch(current_line, max_line_len - 1, ACS_LRCORNER);
  current_line++;

  /* Print #5 */
  for(int i = 0; i < s->side_len; i++){
    move(current_line, s->side_len + 1);
    addch(ACS_VLINE);
    
    for(int j = 0; j < s->side_len; j++){
      color c = s->faces[NUM_FACES - 1][get_coord(j, i, s->side_len)];
      int actual_color = ctoa(c);

      addch(actual_color);
    }

    addch(ACS_VLINE);
    current_line++;
  }

  //Bottom of #5
  move(current_line, s->side_len + 1);
  addch(ACS_LLCORNER);
  for(int i = 0; i < s->side_len; i++){
    addch(ACS_HLINE);
  }
  addch(ACS_LRCORNER);
}

/********************
 * HELPER FUNCTIONS *
 ********************/

void rotate_face(color *face, int side_len){
  if(face == NULL){
    return;
  }

  //This is just a matrix rotation
  for(int i = 0; i < side_len / 2; i++){
    for (int j = i; j < side_len - i - 1; j++){
      int coord = get_coord(j, side_len - i - 1, side_len);
      int prev_coord = get_coord(i, j, side_len);
      
      //Need a temp variable to move in-place
      int temp = face[prev_coord];
 
      //Right to Upper
      face[prev_coord] = face[coord];
      
      //Bottom to Right
      prev_coord = coord;
      coord = get_coord(side_len - i - 1, side_len - j - 1, side_len);
      face[prev_coord] = face[coord];
 
      //Left to Bottom
      prev_coord = coord;
      coord = get_coord(side_len - j - 1, i, side_len);
      face[prev_coord] = face[coord];
 
      //Temp to Left
      face[coord] = temp;
    }
  }
}

void copy_side(color *dest,
               int dest_side,
               color *source,
               int source_side,
               int side_len,
	       int depth){
  //Rotate both these sides to the top for easy copying
  if(dest_side != 0){
    for(int i = 0; i + dest_side < 4; i++){
      rotate_face(dest, side_len);
    }
  }
  if(source_side != 0){
    for(int i = 0; i + source_side < 4; i++){
      rotate_face(source, side_len);
    }
  }

  //Make the copy
  memcpy(dest + (depth * side_len), source + (depth * side_len), side_len);

  //Rotate them back
  if(dest_side != 0){
    for(int i = 0; i < dest_side; i++){
      rotate_face(dest, side_len);
    }
  }
  if(source_side != 0){
    for(int i = 0; i < source_side; i++){
      rotate_face(source, side_len);
    }
  }
}

int ctoa(color c){
  switch(c){
  case 0:
    return 'B' | COLOR_PAIR(CP_BLUE_BLACK);
  case 1:
    return 'O' | COLOR_PAIR(CP_ORANGE_BLACK);
  case 2:
    return 'W' | COLOR_PAIR(CP_WHITE_BLACK);
  case 3:
    return 'R' | COLOR_PAIR(CP_RED_BLACK);
  case 4:
    return 'Y' | COLOR_PAIR(CP_YELLOW_BLACK);
  default:
    return 'G' | COLOR_PAIR(CP_GREEN_BLACK);
  }
}

int get_face(char *str){
  if(str == NULL){
    return -1;
  }

  //Get an all-lowercase version of str
  int len = strlen(str);
  char *lower = Calloc(len, sizeof(char));
  int lower_i = 0;
  for(int i = 0; str[i] != '\0'; i++){
    //Cut off any leading digits (and non-alpha characters in general)
    if(isalpha(str[i])){
      lower[lower_i] = tolower(str[i]);
      lower_i++;
    }
  }

  //Start comparisons with pre-defined
  if(strcmp(lower, "b") == 0){
    return 0;
  }
  else if(strcmp(lower, "l") == 0){
    return 1;
  }
  else if(strcmp(lower, "u") == 0){
    return 2;
  }
  else if(strcmp(lower, "r") == 0){
    return 3;
  }
  else if(strcmp(lower, "d") == 0){
    return 4;
  }
  else if(strcmp(lower, "f") == 0){
    return 5;
  }
  
  return -1;
}


int get_depth(char *str){
  if(str == NULL){
    return -1;
  }

  /* We're simply looking for a number at the start of the string, which
   * we will subtract by one if found, and return 0 if not found. 
   */
  int result = atoi(str);
  if(result > 0){
    result--;
  }
  
  return result;
}

bool is_clockwise(char *str){
  if(str == NULL){
    return true;
  }

  int len = strlen(str);
  if(len < 1){
    return true;
  }

  return str[len - 1] != '\'';
}
