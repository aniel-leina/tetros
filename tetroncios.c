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

/*void delay_lock(Board *board, Tetroncios *tetron, bool just_moved)
{
  //static int reset_count = 15;
  //static int frame_count = 13; // mitad de frame rate
  // static bool first_time = true; // ya que deepest_y no se puede iniciar con variable
  //static int deepest_y = 0;

  if (first_time) {
    deepest_y = tetron->pos.y;
    first_time = false;
  }

  tetron->pos.y++;

  if (ilegal_move(board, tetron)) {
    tetron->extra.dl_lck_grnd = true;
  } else if (tetron->pos.y >= tetron->extra.dl_lck_dpst_y){
    // tetron->extra.dl_lck_grnd = false;
  }
  
  tetron->pos.y--;

  if (!tetron->extra.dl_lck_grnd) {
    tetron->extra.dl_lck_rst_cnt = 15;
    tetron->extra.dl_lck_frm_cnt = (int) (FRAME_COUNT / 2);
    tetron->extra.dl_lck_dpst_y = tetron->pos.y;
    return; // asumimos que algun momento el grounded se setea
  }

  
  if (tetron->extra.dl_lck_frm_cnt == 0) {
    add_one_piece(board, tetron);
    reset_piece_pos(board, tetron);
    tetron->extra.dl_lck_grnd = false;
    return; // gestionar el resultado de que se bloquee reinicuar
  }

  if (just_moved && tetron->extra.dl_lck_rst_cnt > 0)
  {
    tetron->extra.dl_lck_frm_cnt = (int) (FRAME_COUNT / 2);
    tetron->extra.dl_lck_rst_cnt--;
  }

  if (tetron->pos.y > tetron->extra.dl_lck_dpst_y) {
    //reset_count = 15;
    //frame_count = 13; si lo pongo arriba tiene el mismo efecto y es mas seguro
    //deepest_y = tetron->pos.y;
    tetron->extra.dl_lck_grnd = false;
  }

  tetron->extra.dl_lck_frm_cnt--;
} */

void delay_lock(Board *board, Tetroncios *tetron, bool just_moved)
{
  tetron->pos.y++;
  bool settled = ilegal_move(board, tetron);
  tetron->pos.y--; 

  // si esta en modo cuenta atras osea que ha tocado suelo y esta en ese lapso de tiempo de intentar salverse 
  // si se mueve se le resetea el contador pero a cambio se le van quietando las oportunidades de maniobrar
  if (just_moved && tetron->extra.dl_lck_grnd) {
    tetron->extra.dl_lck_frm_cnt = (int) (FRAME_COUNT / 2);
    tetron->extra.dl_lck_rst_cnt--; 
  }

  // si baja  mas abajo del record de mas hondo que ha ido se resetea todo y no esta en modo cuenta atras
  if (tetron->pos.y > tetron->extra.dl_lck_dpst_y) {
    tetron->extra.dl_lck_dpst_y = tetron->pos.y;
    tetron->extra.dl_lck_rst_cnt = DL_CHANCES;
    tetron->extra.dl_lck_grnd = false;
  }

  // tocando suelo
  if (settled) {
    // no tiene mas margen de maniobra o ya ha maniobrado mas del limite
    if (tetron->extra.dl_lck_frm_cnt <= 0 || tetron->extra.dl_lck_rst_cnt <= 0) {
      // bloquearlo
      add_one_piece(board, tetron);
      reset_piece_pos(board, tetron);
      tetron->extra.dl_lck_dpst_y = tetron->pos.y;
      tetron->extra.dl_lck_rst_cnt = DL_CHANCES;
      tetron->extra.dl_lck_grnd = false;
      tetron->extra.dl_lck_frm_cnt = (int) (FRAME_COUNT / 2);
      return;
    }
    // si toca suelo y esta en una y q no es mas baja que el y mas bajo guardado
    if (tetron->pos.y <= tetron->extra.dl_lck_dpst_y) {
      tetron->extra.dl_lck_grnd = true;
    }
    // descuenta del contador en el que puede maniobrar
    tetron->extra.dl_lck_frm_cnt--;
  } else if (tetron->extra.dl_lck_rst_cnt > 0) {
    // si esta en aire osea no settled la cuenta atras se reinicia y conjela
    tetron->extra.dl_lck_frm_cnt = (int) (FRAME_COUNT / 2);
  }

  /*
  if (settled && (tetron->extra.dl_lck_frm_cnt <= 0 || tetron->extra.dl_lck_rst_cnt <= 0)) {
    // bloquearlo osea añadir y subir 
  }
  //
  if (settled && tetron->pos.y < tetron->extra.dl_lck_dpst_y) {
    tetron->extra.dl_lck_grnd = true;
  }
  //
  if (settled) {
    tetron->extra.dl_lck_frm_cnt--;
  } else {
    tetron->extra.dl_lck_frm_cnt = (int) (FRAME_COUNT / 2);
  }*/
}

void draw_tetron(Tetroncios *tetron)
{
  draw_one_piece(tetron);
  flush();
}

/*void update_tetron_v1(Board *board, Tetroncios *tetron, char comando)
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

  
  if (!ilegal_move(board, &cpy_tetron))
  {
    *tetron = cpy_tetron;
    just_moved = true;
  } else if (tetron->pos.y == initial_y) tetron->grounded = true;

  // solo has movido izq derecha o girado
  
  delay_lock(board, tetron, just_moved);
}*/

void update_tetron(Board *board, Tetroncios *tetron, char comando)
{
  Tetroncios backup_tetron = *tetron;
  // int initial_y = tetron->pos.y;
  bool just_moved = false;
  bool first_check = false; // this checks are for determinig if the player inputed smth to move and succeded
  bool second_check = false;

  // moved with intention
  bool input_moved = move_piece_rest(board, tetron, comando) || move_piece_x(board, tetron, comando);  

  // first check
  if (ilegal_move(board, tetron)) {
    *tetron = backup_tetron;
  } else {
    backup_tetron = *tetron;
    first_check = input_moved; // only here since if x moved and y no the counter will substract
    // if x moved and y also grounded false will reset anyways so i can maybe add it 
    // there just for clarity but is not neccesary
  }
  // end first check

  // moved with intention gravity doesnt count
  input_moved = move_piece_y(board, tetron, comando);

  // second check
  if (ilegal_move(board, tetron)) {
    *tetron = backup_tetron;
    tetron->extra.dl_lck_grnd = true;
  } else {
    //tetron->grounded = false;
    second_check = input_moved;
  }
  // end second check

  just_moved = first_check || second_check; // intentional movement
  delay_lock(board, tetron, just_moved);
  
  move_piece_resets(board, tetron, comando);
}

void move_piece_resets(Board *board, Tetroncios *tetron, char comando)
{
  switch (comando)
  {
    case ' ':
      add_one_piece(board, tetron);
      reset_piece_pos(board, tetron);
      break;
    case 'r':
      reset_piece_pos(board, tetron);
      break;
  }
  
}

bool move_piece_y(Board *board, Tetroncios *tetron, char comando)
{
  switch (comando)
    {
     case 'w':
      general_move(tetron, 0, -1);
      return true;
      break;
    case ' ':
      hard_drop(board, tetron, 0);
      break;
    case 's':
      soft_drop_fixed(tetron);
      return true;
      break;
    default:
      gravity(tetron, 0);
      return false;
      break;
    }
}

bool move_piece_x(Board *board, Tetroncios *tetron, char comando)
{
  switch (comando) // so that gravity can still affect even if you  move side ways
  {
    case 'd':
      general_move(tetron, 1, 0);
      return true;
      break;
    case 'a':
      general_move(tetron, -1, 0);
      return true;
      break;
  }
  return false;
}

bool move_piece_rest(Board *board, Tetroncios *tetron, char comando)
{
  switch (comando)
  {
    case 'j': case 'J':
      rotate(tetron, false);
      return true;
      break;
    case 'k': case 'K':
      rotate(tetron, true);
      return true;
      break;
  }
  return false;
}

void hard_drop(Board *board, Tetroncios *tetron, int iteration)
{
  if (ilegal_move(board, tetron)) {
    if (iteration == 0) return;
    tetron->pos.y--;
    return;
  } else {
    tetron->pos.y++;
    hard_drop(board, tetron, ++iteration);
  }
}

void soft_drop_fixed(Tetroncios *tetron)
{
  //static int frames_since_last = 2 - 1;

   if (tetron->extra.sft_drp_frm_cnt == 0)
  {
    general_move(tetron, 0, 1);
    tetron->extra.sft_drp_frm_cnt = 2 - 1; 
  } else 
  {
    tetron->extra.sft_drp_frm_cnt--;
  }
}

void gravity(Tetroncios *tetron, int level)
{
  // algo para diferentes levels meteria aqui mas tarde

  // static int tetros->extra.grvt_frm_cont = 20 - 1;

  if (tetron->extra.grvt_frm_cnt == 0) {
    general_move(tetron, 0, 1);
    tetron->extra.grvt_frm_cnt = 20 - 1; // this would change depending on the level later on
  } else {
    tetron->extra.grvt_frm_cnt--;
  }
}

void reset_piece_pos(Board *board, Tetroncios *tetron)
{
  // x since one bloxk is 2 x then if i want to reset the piece to the half of the board the 2 cancel each other out
  // y at the top
  //
  //tetron->pos = board->pos;

  tetron->pos.y = board->pos.y + 1;
  tetron->extra.dl_lck_dpst_y = tetron->pos.y;

  tetron->pos.x = board->pos.x + (CELLS_WIDTH / 2 - tetron->size / 2) * 2; // board origin + (ideal pos relative to the origin) * 2 to fix the one block = 2 x
  if (ilegal_move(board, tetron)) clean_board(board);
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

  Tetroncios piece = 
    {
      .size = size, 
      .pos = pos,
      .extra = mk_miscellanous(pos.y)
    };

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

Miscellanous mk_miscellanous(int deepest_y)
{
  Miscellanous ms =
    {
      .sft_drp_frm_cnt = 2,
      .grvt_frm_cnt = (int) (FRAME_COUNT * 0.8),
      .dl_lck_rst_cnt = DL_CHANCES,
      .dl_lck_frm_cnt = (int) (FRAME_COUNT / 2),
      .dl_lck_dpst_y = deepest_y,
      .dl_lck_grnd = false
    };

  return ms;
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

