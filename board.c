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
  Tetroncios piece = mk_tetroncios(&board, I);
  choose_next_piece(&board, &piece);
  char c;

  while (true)
  {
    c = read_key();

    if (c == 'q') break;
    update_tetron(&board, &piece, c);
    clean_filled_lines(&board);
    clear_screen();

    draw_board(&board);
    draw_tetron(&board, &piece);

    usleep(1000000/25);
  }

  exit(0);
}

bool one_line_filled(BlockTypes line[], int size)
{
  for (int i = 0; i < size; i++)
  {
    if (line[i] == BOARD_WS) return false;
  }

  return true;
}

void erase_one_line(Board *board, int line_index)
{
  for (int i = line_index; i > 0; i--)
  {
    for (int j = 0; j < CELLS_WIDTH; j++)
    {
      swap(&board->cells[i][j], &board->cells[i-1][j]);
    }
  }

  for (int i = 0; i < CELLS_WIDTH; i++)
  {
    board->cells[0][i] = BOARD_WS;
  }
}

int clean_filled_lines(Board *board) // opero en el array ya que se avanza hacia adelante pero se borra cosas de atras osea no molesta la parte de adelante
{
  int filled_count = 0;
  for (int i = 0; i < CELLS_HIGH; i++)
  {
    if (one_line_filled(board->cells[i], CELLS_WIDTH))
    {
      erase_one_line(board, i);
      filled_count++;
    }
  }

  return filled_count;
}

bool ilegal_move(Board *board, Tetroncios *tetron) // this will receive a copy reference
{
  if (check_collisions(board, tetron)) return true;
  if (out_of_bounds(board, tetron)) return true;
}

bool check_collisions(Board *board, Tetroncios *tetron)
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

      if (board->cells[pos_y][pos_x] != BG_W) return true;
    }
  }
  return false;
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

/*void add_one_piece(Board *board, Tetroncios *tetron)
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
}*/

void add_one_piece(Board *board, Tetroncios *tetron)
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
  //bool zig_zag = false;
  int pos_y, pos_x;
  for (int i = 0; i < CELLS_HIGH; i++)
  {
    pos_y = board->pos.y + i;
    for (int j = 0; j < CELLS_WIDTH; j++)
    {
      pos_x = board->pos.x + j * 2;
      draw_one_block(board->cells[i][j], pos_x, pos_y);
      //if (zig_zag) 
      //{
      //  draw_one_block(Z, pos_x, pos_y);
      //  zig_zag = !zig_zag;
      //} else 
      //{
      //  draw_one_block(S, pos_x, pos_y);
      //  zig_zag = !zig_zag;
      //}
    }
  }

  flush();
}

void clean_board(Board *board)
{
  for (int i = 0; i < CELLS_HIGH; i++)
  {
    for (int j = 0; j < CELLS_WIDTH; j++)
    {
      board->cells[i][j] = (BlockTypes) BOARD_WS;
    }
  }
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
      board.cells[i][j] = (BlockTypes) BOARD_WS;
    }
  }

  board.bag = mk_random_bag(&board, NUMBER_OF_PIECES);

  return board;
}

Bag mk_random_bag(Board *board, int n_pieces) // board es para que las piezas creadas tengan coordenadas buenas
{
  BlockTypes linear_bag[n_pieces];
  Bag random_bag = 
    {
      .next_piece = 0
    };

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
