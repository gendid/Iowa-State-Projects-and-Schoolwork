#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ncurses.h>

#include "board.h"
#include "unit.h"
#include "io.h"

void new_board(board*b);

void init_board(board* b)
{
  new_board(b);
}

void new_board(board* b)
{
  board* blank= new board();
  b=blank;
}

void place_piece_p1(board* b)
{
  int x=0;
  int y=6;
  char c;


  while(!b->player1pieces.empty())
    {
      io_display(b);
      mvaddch(y+1, x, '*');
      refresh();
      mvprintw(0, 0, "Player 1, place a %s, use r to place, wasd to move", b->player1pieces.front()->returnName().c_str());

  switch((c = getch()))
    {
    case 'q':
      io_reset_terminal();
      break;
    case 'w':
      if (y>0)
	{
	  y--;
	}
      break;
    case 'a':
      if (x>0)
	{
	  x--;
	}
      break;
    case 's':
      if (y<9)
	{
	  y++;
	}
      break;
    case 'd':
      if (x<9)
	{
	  x++;
	}
      break;
    case 'r':
      if((b->spaces[y][x]==0)&&(y>=6)&&(y<10))
	{
	  b->spaces[y][x]=b->player1pieces.front();
	  b->player1pieces.erase(b->player1pieces.begin());
	}
      else
	{
	  clear();
	  if(b->spaces[y][x]!=0)
	    {
	      mvprintw(0,0,"Space occupied");
	    }
	  else
	    {
	      mvprintw(0,0,"Invalid starting space");
	    }
	  getch();
	}

    }
  if(c=='q')
    {
      break;
    }
    }

  refresh();
}

void place_piece_p2(board* b)
{
  int x=0;
  int y=0;
  char c;
  b->playerTurn=2;

  while(!b->player2pieces.empty())
    {
      io_display(b);
      mvaddch(y+1, x, '*');
      refresh();
      mvprintw(0, 0, "Player 2, place a %s, use r to place, wasd to move", b->player2pieces.front()->returnName().c_str());

  switch((c = getch()))
    {
    case 'q':
      io_reset_terminal();
      break;
    case 'w':
      if (y>0)
	{
	  y--;
	}
      break;
    case 'a':
      if (x>0)
	{
	  x--;
	}
      break;
    case 's':
      if (y<9)
	{
	  y++;
	}
      break;
    case 'd':
      if (x<9)
	{
	  x++;
	}
      break;
    case 'r':
      if((b->spaces[y][x]==0)&&(y>=0)&&(y<=3))
	{
	  b->spaces[y][x]=b->player2pieces.front();
	  b->player2pieces.erase(b->player2pieces.begin());
	}
      else
	{
	  clear();
	  if(b->spaces[y][x]!=0)
	    {
	      mvprintw(0,0,"Space occupied");
	    }
	  else
	    {
	      mvprintw(0,0,"Invalid starting space");
	    }
	  getch();
	}

    }
  if(c=='q')
    {
      break;
    }
    }

  refresh();
}

void delete_board(board*b)
{
  free(b);
}
