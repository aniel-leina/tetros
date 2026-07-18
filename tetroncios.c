#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "utils.h"
#include "tetroncios.h"
#include "myInput.h"
#include "cursor.h"
#include "assets.h"
#include "board.h"


/*int main (void)
{
  init_terminal();
  Tetroncios piece = mk_tetroncios(T, 1, 1);

  while (1)
  {
    char c = read_key();

    if (c == 'q') break;

    update_tetron(&piece, c);
    draw_tetron(&piece);
    
    usleep(1000000/25);
  }

  exit(0);
}*/

void delay_lock(Board *board, Tetroncios *tetron, bool just_moved)
{
  static int reset_count = 15;
  static int frame_count = 13; // mitad de frame rate
  static bool first_time = true; // ya que deepest_y no se puede iniciar con variable
  static int deepest_y = 0;

  if (first_time)
  {
    deepest_y = tetron->pos.y;
    first_time = false;
  }

  if (!tetron->grounded) {
    reset_count = 15;
    frame_count = 13;
    deepest_y = tetron->pos.y;
    return; // asumimos que algun momento el grounded se setea
  }
  if (frame_count == 0) {
    add_one_piece(board, tetron);
    reset_piece_pos(board, tetron);
    tetron->grounded = false;
    return; // gestionar el resultado de que se bloquee reinicuar
  }

  if (just_moved && reset_count > 0)
  {
    frame_count = 13;
    reset_count--;
  }

  if (tetron->pos.y > deepest_y) {
    //reset_count = 15;
    //frame_count = 13; si lo pongo arriba tiene el mismo efecto y es mas seguro
    //deepest_y = tetron->pos.y;
    tetron->grounded = false;
  }

  frame_count--;
}

void draw_tetron(Tetroncios *tetron)
{
  draw_one_piece(tetron);
  flush();
}

void update_tetron(Board *board, Tetroncios *tetron, char comando)
{
  Tetroncios cpy_tetron = *tetron;
  int initial_y = tetron->pos.y; // to check if i need to ground
  bool just_moved = false; // this is to help the delayed lock

  move_piece(board, &cpy_tetron, comando);

  if (!ilegal_move(board, &cpy_tetron))
  {
    *tetron = cpy_tetron;
    just_moved = true;
  } else {
    cpy_tetron = *tetron; // para cancelar el antiguo movimiento
  } 

  switch (comando)
    {
      case 'w': case ' ': case 's': case 'r':
        break;
      default:
        gravity(&cpy_tetron, 0);
    }

  if (!ilegal_move(board, &cpy_tetron))
  {
    *tetron = cpy_tetron;
    just_moved = true;
  } else if (tetron->pos.y == initial_y) tetron->grounded = true;

  // solo has movido izq derecha o girado
  
  delay_lock(board, tetron, just_moved);
}

void move_piece(Board *board, Tetroncios *tetron, char comando)
{
  switch (comando)
  {
    case 'j': case 'J':
      rotate(tetron, false);
      break;
    case 'k': case 'K':
      rotate(tetron, true);
      break;
  }

  switch (comando) // so that gravity can still affect even if you  move side ways
  {
    case 'd':
      general_move(tetron, 1, 0);
      break;
    case 'a':
      general_move(tetron, -1, 0);
      break;
  }

  switch (comando)
  {
    case 'w':
      general_move(tetron, 0, -1);
      break;
    case ' ':
      hard_drop(board, tetron);
      add_one_piece(board, tetron);
      reset_piece_pos(board, tetron);
      break;
    case 's':
      soft_drop_fixed(tetron);
      break;
    case 'r':
      reset_piece_pos(board, tetron);
      break;
  }
}

void hard_drop(Board *board, Tetroncios *tetron)
{
  if (ilegal_move(board, tetron)) {
    tetron->pos.y--;
    return;
  } else {
    tetron->pos.y++;
    hard_drop(board, tetron);
  }
}

void soft_drop_fixed(Tetroncios *tetron)
{
  static int frames_since_last = 2 - 1;

   if (frames_since_last == 0)
  {
    general_move(tetron, 0, 1);
    frames_since_last = 2 - 1; 
  } else 
  {
    frames_since_last--;
  }
}

void gravity(Tetroncios *tetron, int level)
{
  // algo para diferentes levels meteria aqui mas tarde

  static int frames_since_last = 20 - 1;

  if (frames_since_last == 0)
  {
    general_move(tetron, 0, 1);
    frames_since_last = 20 - 1; // this would change depending on the level later on
  } else 
  {
    frames_since_last--;
  }
}

void reset_piece_pos(Board *board, Tetroncios *tetron)
{
  // x since one bloxk is 2 x then if i want to reset the piece to the half of the board the 2 cancel each other out
  // y at the top
  //
  tetron->pos = board->pos;

  tetron->pos.x = board->pos.x + (CELLS_WIDTH / 2 - tetron->size / 2) * 2; // board origin + (ideal pos relative to the origin) * 2 to fix the one block = 2 x 
}
void general_move(Tetroncios *tetron, int x_increment, int y_increment)
{
  x_increment *= 2;

  tetron->pos.y += y_increment;
  tetron->pos.x += x_increment;
}

void rotate(Tetroncios *tetron, bool clockwise)
{
  int size = tetron->size;

  if (clockwise)
  {
    switch (size)
    {
      case 2:
        transpose(size, tetron->set.r2);
        flip_rows_matrix(size, tetron->set.r2);
        break;
      case 3:
        transpose(size, tetron->set.r3);
        flip_rows_matrix(size, tetron->set.r3);
        break;
      case 4:
        transpose(size, tetron->set.r4);
        flip_rows_matrix(size, tetron->set.r4);
        break;
    }
  } 
  else 
  {
    switch (size)
    {
      case 2:
        flip_rows_matrix(size, tetron->set.r2);
        transpose(size, tetron->set.r2);
        break;
      case 3:
        flip_rows_matrix(size, tetron->set.r3);
        transpose(size, tetron->set.r3);
        break;
      case 4:
        flip_rows_matrix(size, tetron->set.r4);
        transpose(size, tetron->set.r4);
        break;
    }
  }
}

void flip_rows_matrix(int size, BlockTypes matrix[size][size])
{
  for (int i = 0; i < size; i++)
  {
    flip_row(size, matrix[i]);
  }
}

void flip_row(int size, BlockTypes array[size])
{
  int half_size = (int) (size / 2); // i want floor the division

  for (int i = 0; i < half_size; i++)
  {
    swap(&array[i], &array[size - 1 - i]);
  }
}

void transpose(int size, BlockTypes matrix[size][size])
{
  for (int i = 0; i < size; i++)
  {
    for (int j = i + 1; j < size; j++)
    {
      swap(&matrix[i][j], &matrix[j][i]);
    }
  }
}

void swap(BlockTypes *a, BlockTypes *b)
{
  BlockTypes temp = *a;
  *a = *b;
  *b = temp;
}

void draw_one_piece(Tetroncios *tetron)
{
  for (int i = 0; i < tetron->size; i++)
  {
    int pos_y = tetron->pos.y + i;
    for (int j = 0; j < tetron->size; j++)
    {
      int pos_x = tetron->pos.x + j * 2;
      switch (tetron->size)
      {
        case 2:
          draw_one_block(tetron->set.r2[i][j], pos_x, pos_y);
          break;
        case 3:
          draw_one_block(tetron->set.r3[i][j], pos_x, pos_y);
          break;
        case 4:
          draw_one_block(tetron->set.r4[i][j], pos_x, pos_y);
          break;
      }
    }
  }
  flush();
}

Tetroncios mk_tetroncios(Board *board, BlockTypes te_type)
{
  Point pos;
  int size;

  switch (te_type)
  {
    case I: size = 4; break;
    case J: case L: case S: case T: case Z: size = 3; break;
    case O: size = 2; break;
    default:
      printf("L23 archivo tetroncios, tipo no registrado\n");
      break;
  } 

  pos = board->pos;

  pos.x = board->pos.x + (CELLS_WIDTH / 2 - size / 2) * 2;

  Tetroncios piece = {.size = size, .pos = pos, .grounded = false};

  switch (te_type)
  {
      case I:
          memcpy(piece.set.r4, (BlockTypes[4][4]){
              {N, N, N, N},
              {I, I, I, I},
              {N, N, N, N},
              {N, N, N, N}
          }, sizeof(BlockTypes[4][4]));
          break;
        
      case J:
          memcpy(piece.set.r3, (BlockTypes[3][3]){
              {J, N, N},
              {J, J, J},
              {N, N, N}
          }, sizeof(BlockTypes[3][3]));
          break;
        
      case L:
          memcpy(piece.set.r3, (BlockTypes[3][3]){
              {N, N, L},
              {L, L, L},
              {N, N, N}
          }, sizeof(BlockTypes[3][3]));
          break;
        
      case O:
          // La pieza O es un bloque estático de 2x2
          memcpy(piece.set.r2, (BlockTypes[2][2]){
              {O, O},
              {O, O}
          }, sizeof(BlockTypes[2][2]));
          break;
        
      case S:
          memcpy(piece.set.r3, (BlockTypes[3][3]){
              {N, S, S},
              {S, S, N},
              {N, N, N}
          }, sizeof(BlockTypes[3][3]));
          break;
        
      case T:
          memcpy(piece.set.r3, (BlockTypes[3][3]){
              {N, T, N},
              {T, T, T},
              {N, N, N}
          }, sizeof(BlockTypes[3][3]));
          break;
        
      case Z:
          memcpy(piece.set.r3, (BlockTypes[3][3]){
              {Z, Z, N},
              {N, Z, Z},
              {N, N, N}
          }, sizeof(BlockTypes[3][3]));
          break;
  }

  return piece;
}

void draw_one_block(BlockTypes te_type, int x, int y)
{
  switch (te_type)
  {
    case N:
      return;
    case BG_W:
      // move_cursor(x,y); // later on erase 
      printf(BG_GRIS);
      break;
      //return;
    case I:
      printf(CIAN);
      break;
    case J:
      printf(AZUL);
      break;
    case L:
      printf(NARANJA);
      break;
    case O:
      printf(AMARILLO);
      break;
    case S:
      printf(VERDE);
      break;
    case T:
      printf(MORADO);
      break;
    case Z:
      printf(ROJO);
      break;
  }
  move_cursor(x, y);
  printf(BLOCK);
  printf(RESET);
}

