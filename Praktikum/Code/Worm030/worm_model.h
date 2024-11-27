// A simple variant of the game Snake
//
// Used for teaching in classes
//
// Author:
// Franz Regensburger
// Ingolstadt University of Applied Sciences
// (C) 2011
//
// The worm model

#ifndef _WORM_MODEL_H
#define _WORM_MODEL_H

#include "worm.h"
#include <stdbool.h>

// Directions
enum WormHeading {
    WORM_UP,
    WORM_DOWN,
    WORM_LEFT,
    WORM_RIGHT,
  };

extern enum ResCode initializeWorm(int len_max, int headpos_y, int headpos_x,
                                   enum WormHeading dir, enum ColorPair color);
extern void showWorm();
extern void cleanWormTail();
extern void moveWorm(enum GameState *agame_state);
extern bool isInUseByWorm(int new_headpos_y, int new_headpos_x);

// Setters
extern void setWormHeading(enum WormHeading dir);

#endif  // #define _WORM_MODEL_H
