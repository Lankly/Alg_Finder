#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <curses.h>
#include <math.h>
#include "helpers.h"
#include "state.h"

#define MAX_INPUT_LEN 16
#define HISTORY_LEN 12
#define HISTORY_ITEM_MAX_LEN MAX_INPUT_LEN

/*********************
 * Private variables *
 *********************/
int side_len = 3;


/********************
 * Helper functions *
 ********************/

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

  int max_lines_to_print = max_lines_to_print = MIN(HISTORY_LEN, side_len * 4);
  
  for(int i = 0; i < HISTORY_LEN && i < max_lines_to_print; i++){
    if(history[i] == NULL){
      break;
    }
    mvaddstr(i, x_coord, history[i]);
  }
}

void print_move_count(int count, int x_coord, int y_coord){
  if(count < 1){
    return;
  }
  
  //Arbitrarily choose 10 digits
  char *count_as_str = Calloc(10, sizeof(char));
  sprintf(count_as_str, "%u", count);

  mvaddstr(y_coord, x_coord, count_as_str);
  
  free(count_as_str);
}

void free_history(char **history){
  if(history == NULL){
    return;
  }
  for(int i = 0; i < HISTORY_LEN; i++){
    if(history[i] != NULL){
      free(history[i]);
    }
  }
  free(history);
}

bool confirm_restart(int input_line){
  move(input_line, 0);
  clrtoeol();
  addstr("Are you sure you want to restart? y/N");
  int c = getch();

  if(c != 'y' && c != 'Y'){
    return false;
  }

  //Now get the size of the cube they'd like
  move(input_line, 0);
  clrtoeol();
  const char *question = "How big would you like the new cube to be? ";
  int question_len = strlen(question);
  
  //Arbitrarily limit to 99
  int limit = 3, index = 0;
  char *answer = Calloc(limit, sizeof(char));
  addstr(question);
  c = 0;
  while(c != KEY_ENTER && c != '\n'){
    move(input_line, question_len);
    clrtoeol();
    addstr(answer);
    c = getch();

    if(((c == KEY_BACKSPACE) || (c == 127) || (c == 7)) && index > 0){
      index --;
      answer[index] = 0;
    }
    else if(index < limit && isdigit(c)){
      answer[index] = c;
      index++;
    }
  }

  //If they failed to provide a number, fail
  if(strcmp(answer, "") == 0){
    return false;
  }

  //Or if it was negative (Shouldn't be possible, since '-' can't be typed)
  int answer_as_int = atoi(answer);
  if(answer_as_int < 0){
    return false;
  }
  
  side_len = answer_as_int;
  
  free(answer);

  return true;
}

/********
 * Main *
 ********/
int main(int argc, char** argv){
  //Setup
  WIN = initscr();
  timeout(-1);
  curs_set(2);
  cbreak();
  noecho();
  keypad(stdscr, true);
  color_init();
  
  bool help_menu_entered = false;
  bool restart = false;
  int move_count = 0;
  char *input = Calloc(MAX_INPUT_LEN, sizeof(char));
  char **history = Calloc(HISTORY_LEN, sizeof(char *));
  state_t *s = new_state(side_len);

  
  //Main Loop
  while(true){
    clear();
    print_state(s);

    //Print instructions
    int input_line = side_len * 3 + 4;
    const char *input_inst = "Next move: ";
    mvaddstr(input_line + 1, 0, "Help: ?");
    mvaddstr(input_line, 0, input_inst);

    //Print history and move count
    print_history(history, side_len * 4 + 8);
    print_move_count(move_count, side_len * 4 + 8, input_line + 1);
    
    //Setup for user input
    int c = 0;
    int index = 0;
    memset(input, 0, MAX_INPUT_LEN);

    //Handle if we just came from the help screen or not
    if(help_menu_entered){
      help_menu_entered = false;
      if(history[0] != NULL){
	memcpy(input, history[0], MAX_INPUT_LEN);
      }
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
      if(((c == KEY_BACKSPACE) || (c == 127) || (c == 7)) && index > 0){
	index--;
	input[index] = '\0';
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
    //Check if they're restarting
    if(strcmp(input, "n") == 0 || strcmp(input, "N") == 0){
      if(confirm_restart(input_line)){
	free_history(history);
	history = Calloc(HISTORY_LEN, sizeof(char *));
        move_count = 0;
	free_state(s);
	s = new_state(side_len);
      }
      restart = true;
    }

    //Check for help screen
    if(help_menu_entered){
      print_help();
    }
    //If this was a restart, don't process anything.
    else if(restart){
      restart = false;
    }
    //Process the move
    else{
      //Just hitting enter repeats the previous command
      if(strcmp(input, "") == 0 && history[0] != NULL){
	memcpy(input, history[0], MAX_INPUT_LEN);
      }
    
      state_t *temp = make_move(s, input);
      
      //Only add this input to history if it changed the cube
      if(!state_equal(temp, s)){
	log_history(history, input);
        move_count++;
      }
      
      print_state(temp);
      free_state(s);
      s = temp;
    }
  }

  if(s != NULL){
    free_state(s);
  }
  free_history(history);
  if(input != NULL){
    free(input);
  }

  //Stop ncurses
  endwin();
  
  return 0;
}
