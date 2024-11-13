// A simple variant of the game Snake
//
// Used for teaching in classes
//
// Author:
// Franz Regensburger
// Ingolstadt University of Applied Sciences
// (C) 2011
//
// Command line options

#ifndef _OPTIONS_H
#define _OPTIONS_H

#include <stdbool.h>
#include "worm.h"

// A structure for the command line options
struct game_options
{
    int sysworms_nr;            // Number of automatic sysworms
    bool activate_sysworms;     // Are sysworms active
    bool activate_userworm;     // Is userworm active
    int nap_time;               // Time in milliseconds to sleep at the end of level loop
    bool start_single_step;     // Start game in single step mode
    char* start_level_filename; // File name of file describing the board of current level
};

extern void usage();
extern enum ResCodes readCommandLineOptions(struct game_options* somegops, int argc, char* argv[]); 

#endif  // #define _OPTIONS_H
