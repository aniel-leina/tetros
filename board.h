#ifndef BOARD_H 
#define BOARD_H

#include "tetroncios.h"
#include "utils.h"

#define CELLS_HIGH 22
#define CELLS_WIDTH 10
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
Bag mk_random_bag(int n_pieces);
// END MAKE RANDOM BAG 

#endif
