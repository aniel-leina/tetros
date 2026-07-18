#ifndef TETRONCIOS_H
#define TETRONCIOS_H


#include "utils.h"
#include <stdbool.h>

typedef struct board Board;

typedef enum{
  N, // cero
  I, // cian 4
  J, // azul 3
  L, // naranja 3
  O, // amarillo 2
  S, // verde 3
  T, // morado 3
  Z, // rojo 3
  BG_W, // fondo para board;
} BlockTypes;

typedef union {
  BlockTypes r2[2][2];
  BlockTypes r3[3][3];
  BlockTypes r4[4][4];
} Range;

typedef struct tetroncios {
  int size;
  Point pos;
  Range set;
  bool grounded;
} Tetroncios;

// MAKE TETRON 
Tetroncios mk_tetroncios(Board *board, BlockTypes te_type);
// END MAKE TETRON
// UPDATE
void update_tetron(Board *board, Tetroncios *tetron, char comando);
// END UPDATE
// DRAW TETRON 
void draw_tetron(Tetroncios *tetron);
void draw_one_block(BlockTypes te_type, int x, int y);
void draw_one_piece(Tetroncios *tetron);
//END DRAWING TETRON 
//
// LOCK
void delay_lock(Board *board, Tetroncios *tetron, bool just_moved);
// END LOCK
//
// ROTATE 
void rotate(Tetroncios *tetron, bool clockwise);
void swap(BlockTypes *a, BlockTypes *b);
void transpose(int size, BlockTypes matrix[size][size]);
void flip_row(int size, BlockTypes array[size]);
void flip_rows_matrix(int size, BlockTypes matrix[size][size]);
// END ROTATE
// MOVE
void move_piece(Board *board, Tetroncios *tetron, char comando);
void hard_drop(Board *board, Tetroncios *tetron);
void soft_drop_fixed(Tetroncios *tetron);
void gravity(Tetroncios *tetron, int level);
void reset_piece_pos(Board *board, Tetroncios *tetron);
void general_move(Tetroncios *tetron, int x_increment, int y_increment); // y 20 x 40 los limites
// END MOVE 
#endif // !TETRONCIOS_H
