// A simple variant of the game Snake
//
// Used for teaching in classes
//
// Author:
// Franz Regensburger
// Ingolstadt University of Applied Sciences
// (C) 2011
//
// Functions for manipulating automated system worms

#include <curses.h>
#include <stdbool.h>
#include <stdlib.h>

#include "worm.h"
#include "worm_model.h"
#include "board_model.h"
#include "options.h"
#include "messages.h"

#include "sysworm.h"

enum ResCodes initializeSyswormsArray(struct game_options* somegops, struct worm** ptrToSyswormsArray) {
    enum ResCodes res_code = RES_OK;

    // @nnn

    return res_code;
}

void cleanupSyswormsArray(struct worm* ptrToSyswormsArray) {
    // @nnn
}

enum ResCodes
initializeAndShowAutoWorm(struct board* aboard, struct worm* aworm, int len, enum ColorPairs color) {
    enum ResCodes res_code = RES_OK;

    // @nnn

    return res_code;
}

enum ResCodes enableSysworms(struct game_options* somegops, struct board* aboard, struct worm sysworms[]) {
    enum ResCodes res_code = RES_OK;

    // @nnn

    return res_code;
}

void disableSysworms(struct game_options* somegops, struct board* aboard, struct worm sysworms[]) {

}

bool isGoodHeading(struct board* aboard, struct pos next_pos) {
    // @nnn; replace the following line by some useful code
    return false;
}

enum ResCodes computeDirection(struct board* aboard, struct worm* aworm) {
    enum ResCodes res_code = RES_OK;

    // @nnn

    return res_code;
}

