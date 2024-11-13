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

#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "messages.h"
#include "worm.h"
#include "options.h"

void usage() {
    char buf[100];
    sprintf(buf,"Aufruf: worm [-A num] [-a] [-h] [-n ms] [-s] [-u] [ Dateiname ]");
    showDialog(buf,"Bitte eine Taste druecken");
}

// Read command line options.
// See manual page getopt(3) for documentation
//
enum ResCodes readCommandLineOptions(struct game_options* somegops,
        int argc, char* argv[]) {
    signed char c;

    // Initialize;
    somegops -> sysworms_nr = 0;
    somegops -> activate_sysworms = false;
    somegops -> activate_userworm = true;
    somegops -> nap_time = NAP_TIME;
    somegops -> start_single_step = 0;
    somegops -> start_level_filename = NULL;

    while((c = getopt(argc, argv, "A:an:su")) != -1)
        switch(c) {
            case('A'):
                somegops -> sysworms_nr = atoi(optarg);
                continue;
            case('a'):
                somegops -> activate_sysworms = true;
                continue;
            case('h'):
                usage();
                return RES_WRONG_OPTION;
            case('n'):
                somegops -> nap_time = atoi(optarg);
                continue;
            case('s'):
                somegops -> start_single_step = true;
                continue;
            case('u'):
                somegops -> activate_userworm = false;
                continue;
            default:
                usage();
                return RES_WRONG_OPTION;
        }
    // Skip all options processed
    argc -= optind;
    argv += optind;

    // Are there any non-option arguments left?
    // In our case at most one argument is allowed
    if (argc > 1 ) {
        usage();
        return RES_WRONG_OPTION;
    }

    // Play it safe. If we like to activate sysworm
    // there must be at least one sysworm.
    // Instead of rejecting we define a single worm
    if ( somegops -> activate_sysworms && somegops -> sysworms_nr <= 0) {
        somegops -> sysworms_nr = 1;
    }
    // The argument is supposed to be the filename of a level description
    if (argc == 1) {
        somegops -> start_level_filename = strdup(argv[0]);
        // Do not forget to free the allocated memory somewhere else!
    }
    return RES_OK; 
}

