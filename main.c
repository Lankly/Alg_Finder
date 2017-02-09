#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <curses.h>
#include "helpers.h"
#include "state.h"

#define MAX_INPUT_LEN 16
#define HISTORY_LEN 12
#define HISTORY_ITEM_MAX_LEN MAX_INPUT_LEN

void log_history(char **history, char *item){
  if(history == NULL || item == NULL){
    return;
  }

  //Move all current logs down
  for(int i = HISTORY_LEN - 2; i >= 0; i--){
    if(history[i + 1] == NULL){
      history[i + 1] = Calloc(HISTORY_ITEM_MAX_LEN + 1, sizeof(char));
    }
    if(history[i] != NULL){
      memcpy(history[i + 1], history[i], HISTORY_ITEM_MAX_LEN);
    }
  }

  //Put the new item at the top of history
  if(history[0] == NULL){
    history[0] = Calloc(HISTORY_ITEM_MAX_LEN + 1, sizeof(char));
  }
  memcpy(history[0], item, HISTORY_ITEM_MAX_LEN);
}

void print_history(char **history, int x_coord){
  if(history == NULL){
    return;
  }

  for(int i = 0; i < HISTORY_LEN && history[i] != NULL; i++){
    mvaddstr(i, x_coord, history[i]);
  }
}

//Main
int main(int argc, char** argv){
  //Setup
  WIN = initscr();
  timeout(-1);
  curs_set(2);
  cbreak();
  noecho();
  keypad(stdscr, true);
  color_init();
  
  int side_len = 3;
  state_t *s = new_state(side_len);
  char *input = Calloc(MAX_INPUT_LEN, sizeof(char));
  char **history = Calloc(HISTORY_LEN, sizeof(char *));
  bool help_menu_entered = false;

  
  //Main Loop
  while(true){
    clear();
    print_state(s);

    //Print instructions
    int input_line = side_len * 3 + 4;
    const char *input_inst = "Next move: ";
    mvaddstr(input_line + 1, 0, "Help: ?");
    mvaddstr(input_line, 0, input_inst);

    //Print history
    print_history(history, side_len * 4 + 8);
    
    //Setup for user input
    int c = 0;
    int index = 0;
    memset(input, 0, MAX_INPUT_LEN);

    //Handle if we just came from the help screen or not
    if(help_menu_entered){
      help_menu_entered = false;
      memcpy(input, history[0], MAX_INPUT_LEN);
      index = strlen(input);
    }
    
    //Read from keyboard until enter is pressed
    while((c != '\n') && (c != KEY_ENTER)){
      /* Rewriting the line every time keeps it looking neat in the event of a
       * double-wide charater, or when we return from the help screen.
       */
      move(input_line, strlen(input_inst));
      clrtoeol();
      addstr(input);
      
      c = getch();

      //Handle backspace
      if((c == KEY_BACKSPACE) || (c == 127) || (c == 7)){
	if(index > 0){
	  index--;
	  input[index] = '\0';
	}
      }
      //Handle question mark
      else if(c == '?'){
	help_menu_entered = true;
	//We want the help page to happen instantly
	break;
      }
      //Then only add the next character if input has enough space
      else if(index < MAX_INPUT_LEN - 1 && (isalnum(c) || c == '\'')){{
	  input[index] = c;
	  addch(c);
	  index++;
	}
      }
    }

    //Check if they tried to quit
    if(strcmp(input, "q") == 0 || strcmp(input, "Q") == 0){
      break;
    }

    //Check for help screen
    if(help_menu_entered){
      print_help();
    }
    else{
      //Just hitting enter repeats the previous command
      if(strcmp(input, "") == 0 && history[0] != NULL){
	memcpy(input, history[0], MAX_INPUT_LEN);
      }
    
      state_t *temp = make_move(s, input);
      
      //Only add this input to history if it changed the cube
      if(!state_equal(temp, s)){
	log_history(history, input);
      }
      
      print_state(temp);
      free(s);
      s = temp;
    }
  }

  if(s != NULL){
    free_state(s);
  }
  if(history != NULL){
    for(int i = 0; i < HISTORY_LEN; i++){
      if(history[i] != NULL){
	free(history[i]);
      }
    }
  }
  free(history);
  free(input);

  //Stop ncurses
  endwin();
  
  return 0;
}
