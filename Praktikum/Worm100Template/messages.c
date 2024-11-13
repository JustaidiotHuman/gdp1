// A simple variant of the game Snake
//
// Used for teaching in classes
//
// Author:
// Franz Regensburger
// Ingolstadt University of Applied Sciences
// (C) 2011
//
// Displaying messages and dialogs

#include <curses.h>

#include "worm.h"
#include "board_model.h"
#include "worm_model.h"
#include "messages.h"

// Clear the entire message area
void clearMessageArea() {
    int i,x;
    for (i = 1; i <= 3; i++) {
        move(LINES -ROWS_RESERVED + i,0);
        for (x = 1; x <= COLS; x++) {
            addch(' ');
        }
    }
}

// Display status about the game in the message area
void showStatus(struct board* aboard, struct worm* aworm) {
    int pos_line1 = LINES -ROWS_RESERVED + 1;
    int pos_line2 = LINES -ROWS_RESERVED + 2;
    int pos_line3 = LINES -ROWS_RESERVED + 3;

    clearMessageArea();
    struct pos headpos = getWormHeadPos(aworm);
    mvprintw(pos_line1, 1,"Anzahl verbleibender Futterbrocken: %2d ", getNumberOfFoodItems(aboard));
    mvprintw(pos_line2, 1,"Wurm ist an Position: y=%3d x=%3d", headpos.y, headpos.x);
    mvprintw(pos_line3, 1,"Laenge des Wurms: %3d", getWormLength(aworm) );
}

// Display a dialog in the message area and wait for confirmation
// String prompt1 is displayed in the second line of the message area
// String prompt2 is displayed in the  third line of the message area
int showDialog(char* prompt1, char* prompt2) {
    int pos_line2 = LINES -ROWS_RESERVED + 2;
    int pos_line3 = LINES -ROWS_RESERVED + 3;

    int ch;

    if (prompt1 == NULL) {
        return RES_FAILED;
    } 

    // Delete lines in the message area
    clearMessageArea();

    // Display message
    mvprintw(pos_line2 , 1,"%s", prompt1);
    if (prompt2 != NULL) {
        mvprintw(pos_line3 , 1,"%s", prompt2);
    }
    refresh();

    nodelay(stdscr, FALSE);
    ch = getch();   // Wait for user to press an arbitrary key
    nodelay(stdscr, TRUE);

    // Delete lines in the message area
    clearMessageArea();

    // Display changes
    refresh();

    // Return code of key pressed
    return ch; 
}
