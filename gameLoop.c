#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#include "myInput.h"
#include "cursor.h"
#include "board.h"
#include "tetroncios.h"

/*int main (void)
{
  init_terminal();

  bool running = true;
  char input;
  time_t previo = time(NULL), ahora = time(NULL);
  while (running) 
  {
    input = read_key();

    if (input != '\0') {printf("%c", input); flush();}
    
    ahora = time(NULL);

    if (input == 'q') running = false;

    if ((ahora - previo) >= 1.0)
    {
      previo = ahora;
      printf("\n");
      flush();
    } 

    usleep(1000000/60);
  }

  exit(0);
}*/ 

typedef struct gameloop {
  bool paused;
  bool running;
  int fps;
} GameLoop;

void init_game(GameLoop *gamel) 
{
  init_terminal();
  srand(time(NULL));

  gamel->paused = false;
  gamel->running = true;
  gamel->fps = FRAME_COUNT;
}

void update(Board *board, Tetroncios *tetron, char comando)
{
  update_tetron(board, tetron, comando);
  update_board(board, comando);
}

void draw(Board *board, Tetroncios *tetron)
{
  draw_board(board);
  draw_tetron(board, tetron);
}

int main(void)
{
  GameLoop gameMaster;
  init_game(&gameMaster);

  Board board = mk_board(CELLS_HIGH, CELLS_WIDTH, 9, 5);
  Tetroncios tetron = mk_tetroncios(&board, I);
  choose_next_piece(&board, &tetron);
  char c;



  while (gameMaster.running) 
  {
    c = read_key();

    if (c == 'q') gameMaster.running = false;

    if (c == 'p') gameMaster.paused = !gameMaster.paused;
    if (!gameMaster.paused) 
    {
      update(&board, &tetron, c);
      clear_screen();
    }

    draw(&board, &tetron);

    usleep(1000000/gameMaster.fps);
  }
}
