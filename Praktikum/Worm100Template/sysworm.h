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

#ifndef _SYSWORM_H
#define _SYSWORM_H

// Dimensions and bounds
#define WORM_INITIAL_SYSWORM_LENGTH 8  // Initial length for sysworms

extern enum ResCodes
initializeSyswormsArray(struct game_options* somegops, struct worm** ptrToSyswormsArray);
extern void cleanupSyswormsArray(struct worm* ptrToSyswormsArray);
extern enum ResCodes
initializeAndShowAutoWorm(struct board* aboard, struct worm* aworm, int len, enum ColorPairs color);
extern enum ResCodes
enableSysworms(struct game_options* somegops, struct board* aboard, struct worm sysworms[]);
extern void disableSysworms(struct game_options* somegops, struct board* aboard, struct worm sysworms[]);
extern bool isGoodHeading(struct board* aboard, struct pos next_pos);
extern enum ResCodes computeDirection(struct board* aboard, struct worm* aworm);

#endif  // ifndef _SYSWORM_H
