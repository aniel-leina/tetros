#include <stdio.h>
#include "myInput.h"
#include <stdbool.h>
#include <time.h>
#include "cursor.h"
#include <stdlib.h>
#include <unistd.h>

int main (void)
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
}
