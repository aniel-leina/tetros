#ifndef TETRONCIOS_H
#define TETRONCIOS_H


#include "utils.h"
#include <stdbool.h>

#define DL_CHANCES 15

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

typedef struct miscellanous {
  int sft_drp_frm_cnt;
  int grvt_frm_cnt;
  int dl_lck_rst_cnt;
  int dl_lck_frm_cnt;
  int dl_lck_dpst_y;
  bool dl_lck_grnd;
} Miscellanous;

typedef struct tetroncios {
  int size;
  int orientation;
  Point pos;
  Range set;
  Miscellanous extra;
} Tetroncios;

// MAKE TETRON 
Tetroncios mk_tetroncios(Board *board, BlockTypes te_type);
void cpy_to_set_piece(Tetroncios *tetron, BlockTypes te_type);
Miscellanous mk_miscellanous(int deepest_y);
// END MAKE TETRON
// UPDATE
void update_tetron(Board *board, Tetroncios *tetron, char comando);
void reset_piece(Board *board, Tetroncios *tetron);
void choose_next_piece(Board *board, Tetroncios *tetron);
// END UPDATE
// DRAW TETRON 
void draw_tetron(Board *board, Tetroncios *tetron);
void draw_one_block(BlockTypes te_type, int x, int y);
void draw_one_block_b(BlockTypes te_type, int x, int y);
void draw_one_piece(Tetroncios *tetron);
void draw_shadow(Board *board, Tetroncios *tetron);
//END DRAWING TETRON 
//
// LOCK
void delay_lock(Board *board, Tetroncios *tetron, bool just_moved);
// END LOCK
//
// ROTATE 
void rotate(Tetroncios *tetron, bool clockwise);
void wall_kick(Board *board, Tetroncios *tetron, bool clockwise);
void swap(BlockTypes *a, BlockTypes *b);
void transpose(int size, BlockTypes matrix[size][size]);
void flip_row(int size, BlockTypes array[size]);
void flip_rows_matrix(int size, BlockTypes matrix[size][size]);
// END ROTATE
// MOVE
bool move_piece_y(Board *board, Tetroncios *tetron, char comando); // devuelve true si hay movimiento intencionado que 
bool move_piece_x(Board *board, Tetroncios *tetorn, char comando); // es parte del metodo
bool move_piece_rest(Board *board, Tetroncios *tetron, char comando);
void move_piece_resets(Board *board, Tetroncios *tetron, char comando);
void hard_drop(Board *board, Tetroncios *tetron, int iteration); // iteration es para evitar que si al resetear bloqueo ahi osea piezas campeando en spawn el menos menos pueda hacer que se mueva hacia arriba
void soft_drop_fixed(Tetroncios *tetron);
void gravity(Tetroncios *tetron, int level);
void reset_piece_pos(Board *board, Tetroncios *tetron);
void general_move(Tetroncios *tetron, int x_increment, int y_increment); // y 20 x 40 los limites
// END MOVE 
#endif // !TETRONCIOS_H
