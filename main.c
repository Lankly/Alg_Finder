#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <curses.h>
#include "helpers.h"
#include "state.h"

#define MAX_INPUT_LEN 16

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
  char *prev_input = Calloc(MAX_INPUT_LEN, sizeof(char));
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

    //Setup for user input
    int c = 0;
    int index = 0;
    memset(input, 0, MAX_INPUT_LEN);

    //Handle if we just came from the help screen or not
    if(help_menu_entered){
      help_menu_entered = false;
      memcpy(input, prev_input, MAX_INPUT_LEN);
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
      if(strcmp(input, "") == 0){
	memcpy(input, prev_input, MAX_INPUT_LEN);
      }
    
      state_t *temp = make_move(s, input);
      print_state(temp);
      free(s);
      s = temp;
    }

    memcpy(prev_input, input, MAX_INPUT_LEN);
  }

  free_state(s);

  //Stop ncurses
  endwin();
  
  return 0;
}
