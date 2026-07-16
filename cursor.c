#include "cursor.h"
#include <stdio.h>

void clear_screen(void)
{
  printf("\033[2J");
}

void hide_cursor(void)
{
  printf("\033[?25l");
}

void show_cursor(void)
{
  printf("\033[?25h");
}

void move_cursor(int x, int y)
{
  printf("\033[%d;%dH", y, x);
}

void enter_good_terminal(void)
{
  printf("\033[?1049h");
  move_cursor(1,1);
}

void exit_good_terminal(void)
{
  printf("\033[?1049l");
}

void flush(void)
{
  fflush(stdout);
}
