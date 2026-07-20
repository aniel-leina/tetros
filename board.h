#ifndef BOARD_H 
#define BOARD_H

#include "tetroncios.h"
#include "utils.h"
#include <stdbool.h>

#define CELLS_HIGH 20
#define CELLS_WIDTH 20
#define NUMBER_OF_PIECES 7
#define BOARD_WS BG_W
#define FRAME_COUNT 25

typedef struct bag {
  Tetroncios list[NUMBER_OF_PIECES];
  int next_piece;
} Bag;


typedef struct board {
  BlockTypes cells[CELLS_HIGH][CELLS_WIDTH];
  Point pos;
  Bag bag;
} Board;

// MAKE BOARD 
Board mk_board(int high, int width, int x, int y);
// END MAKE BOARD 

// MAKE RANDOM BAG 
Bag mk_random_bag(Board *board, int n_pieces);
// END MAKE RANDOM BAG 

// DRAW BOARD
void draw_board(Board *board);
// END DRAW BOARD
// BOARD OPERATIONS
void add_one_piece(Board *board, Tetroncios *tetron);
void add_one_piece_v2(Board *board, Tetroncios *tetron);
bool one_line_filled(BlockTypes line[], int size);
void erase_one_line(Board *board, int line_index);
int clean_filled_lines(Board *board);
void clean_board(Board *board);
// END BOARD OPERATIONS
// MOVEMENT VALIDATION 
bool ilegal_move(Board *board, Tetroncios *tetron); //this will receive the reference of a copy 
bool out_of_bounds(Board *board, Tetroncios *tetron);
bool check_collisions(Board *board, Tetroncios *tetron);
// END MOVEMENT VALIDATION 
#endif
