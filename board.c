#include "tetroncios.h"
#include "utils.h"
#include "cursor.h"
#include "myInput.h"
#include "board.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main (void)
{
  init_terminal();
  srand(time(NULL));

  Board board = mk_board(CELLS_HIGH, CELLS_WIDTH, 1, 1);

  for (int i = 0; i < NUMBER_OF_PIECES; i++)
  {
    draw_tetron(&board.bag.list[i]);

    sleep(1);
  }

  board.bag = mk_random_bag(NUMBER_OF_PIECES);

  for (int i = 0; i < NUMBER_OF_PIECES; i++)
  {
    draw_tetron(&board.bag.list[i]);

    sleep(1);
  }

  exit(0);
}

Board mk_board(int high, int width, int x, int y)
{
  Board board = {
    .pos = (Point) {x, y},
  };

  for (int i = 0; i < high; i++)
  {
    for (int j = 0; j < width; j++)
    {
      board.cells[i][j] = (BlockTypes) N;
    }
  }

  board.bag = mk_random_bag(NUMBER_OF_PIECES);

  return board;
}

Bag mk_random_bag(int n_pieces)
{
  BlockTypes linear_bag[n_pieces];
  Bag random_bag;

  for (BlockTypes i = (BlockTypes) 1; i <= (BlockTypes) n_pieces; i++)
  {
    linear_bag[i - 1] = i;
  }

  int index;
  for (int i = n_pieces; i > 0; i--)
  {
    index = rand() % i;

    random_bag.list[i - 1] = mk_tetroncios(linear_bag[index], 1, 1);

    swap(&linear_bag[i - 1], &linear_bag[index]);
  }

  return random_bag;
}
