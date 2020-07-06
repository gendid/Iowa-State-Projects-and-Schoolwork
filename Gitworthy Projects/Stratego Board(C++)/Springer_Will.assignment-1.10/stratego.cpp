#include <cstdio>
#include <iostream>

#include "board.h"
#include "unit.h"
#include "io.h"




int main(int argc, char* argv[])
{
  board* b=new board();

  new_board(b);

  startGameUnits(b);

  io_init_terminal();

  io_display(b);

  place_piece_p1(b);

  place_piece_p2(b);

  io_reset_terminal();

  return 0;
}
