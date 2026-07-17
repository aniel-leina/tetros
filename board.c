#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "tetroncios.h"
#include "utils.h"
#include "cursor.h"
#include "myInput.h"
#include "board.h"

int main (void)
{
  init_terminal();
  srand(time(NULL));

  Board board = mk_board(CELLS_HIGH, CELLS_WIDTH, 22, 12);
  Tetroncios piece = mk_tetroncios(&board, L);
  char c;

  while (true)
  {
    c = read_key();

    if (c == 'q') break;
    if (c == 'n') add_one_piece(&board, &piece); 

    update_tetron(&board, &piece, c);
    clear_screen();

    draw_board(&board);
    draw_tetron(&piece);

    usleep(1000000/25);
  }

  exit(0);
}

bool ilegal_move(Board *board, Tetroncios *tetron) // this will receive a copy reference
{
  ;
}

bool out_of_bounds(Board *board, Tetroncios *tetron)
{
  int pos_x, pos_y;
  for (int i = 0; i < tetron->size; i++)
  {
    pos_y = (tetron->pos.y - board->pos.y) + i;
    for (int j = 0; j < tetron->size; j++)
    {
      pos_x = (tetron->pos.x - board->pos.x) / 2 + j;

      switch (tetron->size)
      {
        case 2:
          if (tetron->set.r2[i][j] == N) continue;
          break;
        case 3:
          if (tetron->set.r3[i][j] == N) continue;
          break;
        case 4:
          if (tetron->set.r4[i][j] == N) continue;
          break;
      }      
      if (pos_y < 0 || pos_x < 0) return true;
      if (pos_y >= CELLS_HIGH || pos_x >= CELLS_WIDTH) return true;
    }
  }

  return false;
}

void add_one_piece(Board *board, Tetroncios *tetron)
{
  for (int i = 0; i < tetron->size; i++)
  {
    int pos_y = (tetron->pos.y - board->pos.y) + i;
    for (int j = 0; j < tetron->size; j++)
    {
      int pos_x = (tetron->pos.x - board->pos.x) / 2 + j;
      switch (tetron->size)
      {
        case 2:
          if (tetron->set.r2[i][j] == N) break;
          board->cells[pos_y][pos_x] = tetron->set.r2[i][j];
          break;
        case 3:
          if (tetron->set.r3[i][j] == N) break;
          board->cells[pos_y][pos_x] = tetron->set.r3[i][j];
          break;
        case 4:
          if (tetron->set.r4[i][j] == N) break;
          board->cells[pos_y][pos_x] = tetron->set.r4[i][j];
          break;
      }
    }
  }
}

void draw_board(Board *board)
{
  bool zig_zag = false;
  for (int i = 0; i < CELLS_HIGH; i++)
  {
    int pos_y = board->pos.y + i;
    for (int j = 0; j < CELLS_WIDTH; j++)
    {
      int pos_x = board->pos.x + j * 2;
      // draw_one_block(board->cells[i][j], pos_x, pos_y);
      if (zig_zag) 
      {
        draw_one_block(Z, pos_x, pos_y);
        zig_zag = !zig_zag;
      } else 
      {
        draw_one_block(S, pos_x, pos_y);
        zig_zag = !zig_zag;
      }
    }
  }

  flush();
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

  board.bag = mk_random_bag(&board, NUMBER_OF_PIECES);

  return board;
}

Bag mk_random_bag(Board *board, int n_pieces)
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

    random_bag.list[i - 1] = mk_tetroncios(board, linear_bag[index]);

    swap(&linear_bag[i - 1], &linear_bag[index]);
  }

  return random_bag;
}
