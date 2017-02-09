#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <curses.h>
#include "helpers.h"
#include "state.h"

//Main
int main(int argc, char** argv){
  //Setup
  WIN = initscr();
  timeout(-1);
  curs_set(1);
  cbreak();
  noecho();
  keypad(stdscr, true);
  
  int side_len = 3;
  state_t *s = new_state(side_len);

  
  //Main Loop
  while(true){
    clear();
    print_state(s);

    mvaddstr(side_len * 3 + 4, 0, "Rotate face: ");
    char input = getch();
    printf("%c\n", input);

    if('q' == input || 'Q' == input){
      break;
    }
    else if('\n' == input){
      continue;
    }
    
    state_t *temp = make_move(s, atoi(&input), true);
    print_state(temp);
    free(s);
    s = temp;
  }
  
  free_state(s);

  return 0;
}
