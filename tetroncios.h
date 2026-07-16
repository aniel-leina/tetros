#ifndef TETRONCIOS_H
#define TETRONCIOS_H


#include "utils.h"

typedef enum{
  N, // cero
  I, // cian 4
  J, // azul 3
  L, // naranja 3
  O, // amarillo 2
  S, // verde 3
  T, // morado 3
  Z, // rojo 3
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
} Tetroncios;

// MAKE TETRON 
Tetroncios mk_tetroncios(BlockTypes te_type, int x, int y);
// END MAKE TETRON
// UPDATE
void update_tetron(Tetroncios *tetron, char comando);
// END UPDATE
// DRAW TETRON 
void draw_tetron(Tetroncios *tetron);
void draw_one_block(BlockTypes te_type, int x, int y);
void draw_one_piece(Tetroncios *tetron);
//END DRAWING TETRON 
//
// ROTATE 
void rotate(Tetroncios *tetron, bool clockwise);
void swap(BlockTypes *a, BlockTypes *b);
void transpose(int size, BlockTypes matrix[size][size]);
void flip_row(int size, BlockTypes array[size]);
void flip_rows_matrix(int size, BlockTypes matrix[size][size]);
// END ROTATE
// MOVE
void move_piece(Tetroncios *tetron, char comando);
void hard_drop(Tetroncios *tetron);
void soft_drop_fixed(Tetroncios *tetron);
void gravity(Tetroncios *tetron, int level);
void reset_piece_pos(Tetroncios *tetron);
void general_move(Tetroncios *tetron, int x_increment, int y_increment); // y 20 x 40 los limites
// END MOVE 
#endif // !TETRONCIOS_H
