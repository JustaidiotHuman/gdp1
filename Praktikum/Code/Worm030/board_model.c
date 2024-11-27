// *************************************************
// Placing and removing items from the game board
// Check boundaries of game board
// *************************************************

#include "board_model.h"
#include "worm.h"
#include <curses.h>

// Place item onto display.
void placeItem(int y, int x, chtype symbol, enum ColorPair color_pair) {
    move(y, x);                      // Move cursor to (y,x)
    attron(COLOR_PAIR(color_pair));  // Start writing in selected color
    addch(symbol);                   // Store symbol on the virtual display
    attroff(COLOR_PAIR(color_pair)); // Stop writing in selected color
}


int getLastRow() { return LINES - 1; }
int getLastCol() { return COLS - 1; }