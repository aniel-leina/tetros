#ifndef CURSOR_H
#define CURSOR_H

void clear_screen(void);

void hide_cursor(void);

void show_cursor(void);

void move_cursor(int x, int y);

void enter_good_terminal(void);

void exit_good_terminal(void);

void flush(void);

#endif
