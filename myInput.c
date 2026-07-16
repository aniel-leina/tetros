#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include "myInput.h"
#include "cursor.h"

static struct termios original_terminal;

// Function to restore the terminal to its normal state upon exiting
void restore_terminal(void) {
    // 1. Restore original keyboard settings
    tcsetattr(STDIN_FILENO, TCSANOW, &original_terminal);
    
    // 2. Show the cursor and exit the Alternate Screen Buffer
    show_cursor();
    exit_good_terminal();
    flush();
}

// Function to prepare the terminal for the game loop
void init_terminal(void) {
    struct termios modified_terminal;

    // 1. Configure keyboard (termios)
    tcgetattr(STDIN_FILENO, &original_terminal);
    atexit(restore_terminal); // Safety net to avoid a zombie terminal

    modified_terminal = original_terminal;
    modified_terminal.c_lflag &= ~(ICANON | ECHO);
    modified_terminal.c_cc[VMIN] = 0;
    modified_terminal.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &modified_terminal);

    // 2. Configure screen (ANSI Codes)
    enter_good_terminal();
    hide_cursor();
    move_cursor(1,1);
    flush();
}

// Function to read a single key press (non-blocking)
char read_key(void) {
    char key = '\0';
    // Attempt to read 1 byte. If it returns > 0, a key was pressed.
    if (read(STDIN_FILENO, &key, 1) > 0) {
        return key;
    }
    // If no key was pressed, return null.
    return '\0';
}
