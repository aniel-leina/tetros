#include "utils.h"
#include "tetroncios.h"
#include "myInput.h"
#include "cursor.h"
#include "assets.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>


int main (void)
{
  init_terminal();
  Tetroncios piece = mk_tetroncios(Z, 1, 1);

  draw_one_piece(&piece);

  while (1)
  {
    char c = read_key();

    switch (c)
    {
      case 'j': case 'J':
        rotate(&piece, false);
        break;
      case 'k': case 'K':
        rotate(&piece, true);
        break;
      case 'w':
        general_move(&piece, 0, -1);
        break;
      case 's':
        general_move(&piece, 0, 1);
        break;
      case 'd':
        general_move(&piece, 1, 0);
        break;
      case 'a':
        general_move(&piece, -1, 0);
        break;
      case 'q':
        exit(0);
    }

    clear_screen();
    // flush();
    draw_one_piece(&piece);

    flush();

    usleep(1000000/60);
  }

  exit(0);
}

void general_move(Tetroncios *tetron, int x_increment, int y_increment)
{
  x_increment *= 2;

  if (y_increment && (tetron->pos.y + y_increment) >= 1 && (tetron->pos.y + y_increment) <= 20)
  {
    tetron->pos.y += y_increment;
  }

  if (x_increment && (tetron->pos.x + x_increment) >= 1 && (tetron->pos.x + x_increment) <= 20 * 2 - 1)
  {
    tetron->pos.x += x_increment;
  }
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

Tetroncios mk_tetroncios(BlockTypes te_type, int x, int y)
{
  Point pos = {x,y};
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

  Tetroncios piece = {.size = size, .pos = pos};
    

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
      move_cursor(x,y); // later on erase 
      printf("  ");
      return;
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

