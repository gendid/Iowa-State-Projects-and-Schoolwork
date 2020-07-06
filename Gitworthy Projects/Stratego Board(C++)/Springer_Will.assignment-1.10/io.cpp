#include <unistd.h>
#include <ncurses.h>
#include <ctype.h>
#include <stdlib.h>

#include "board.h"
#include "unit.h"
#include "io.h"

//static board *theboard;

void io_init_terminal()
{
  initscr();
  raw();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  start_color();
}

void io_reset_terminal()
{
  endwin();

}

void io_display(board* b)
{
  int x, y;
  clear();
  for (y=0; y<10; y++)
    {
      for(x=0; x<10; x++)
	{
	  if(b->spaces[y][x]==0)
	    {
	      mvaddch(y+1,x,'.');
	    }
	  else
	    {
	      if(b->playerTurn==b->spaces[y][x]->returnPlayer())
		{
		  mvaddch(y+1,x,b->spaces[y][x]->returnSymbol());
		}
	      else
		{
		  mvaddch(y+1,x, '?');
		}
	    }	  
	}
    }

  refresh();

}
