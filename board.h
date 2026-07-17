#ifndef BOARD_H 
#define BOARD_H

#include "tetroncios.h"
#include "utils.h"
#include <stdbool.h>

#define CELLS_HIGH 21
#define CELLS_WIDTH 15
#define NUMBER_OF_PIECES 7

typedef struct bag {
  Tetroncios list[NUMBER_OF_PIECES];
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
void add_one_piece(Board *board, Tetroncios *tetron);
// END DRAW BOARD
// MOVEMENT VALIDATION 
bool ilegal_move(Board *board, Tetroncios *tetron); //this will receive the reference of a copy 
bool out_of_bounds(Board *board, Tetroncios *tetron);
// END MOVEMENT VALIDATION 
#endif
