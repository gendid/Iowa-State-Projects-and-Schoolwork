#ifndef IO_H
# define IO_H

class board;

void io_init_terminal();
void io_reset_terminal();
void io_display(board* b);
void io_handle_input(board* b);

#endif
 
