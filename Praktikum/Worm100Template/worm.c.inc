        // START process sysworms if they are active
        if ( somegops->activate_sysworms ) {
            int w;  // Index for sysworms
            enum GameStates worm_state = WORM_GAME_ONGOING ;
            // For all sysworms
            for (w = 0; w < somegops->sysworms_nr && worm_state == WORM_GAME_ONGOING; w++) {
                // Compute a new direction for the sysworm
                res_code = computeDirection(&theboard, &sysworms[w]);
                if ( res_code != RES_OK ) {
                    enum ColorPairs color;

                    // We are not able to find a new direction for that sysworm
                    // Re-initialize that sysworm
                    removeWorm(&theboard, &sysworms[w]);
                    // Because initializeWorm allocates memory
                    cleanupWorm(&sysworms[w]);
                    // Randomly choose a color from all colors available for sysworms
                    color = FIRST_AUTO_COLP + (rand() % (LAST_AUTO_COLP - FIRST_AUTO_COLP +1 ));
                    // Create that worm again
                    res_code = initializeAndShowAutoWorm(&theboard,&sysworms[w],
                            WORM_INITIAL_SYSWORM_LENGTH,color);
                    if ( res_code != RES_OK ) {
                        // We are unable to place a new sysworm onto the board.
                        // Cancel loops and propagate state
                        end_level_loop = true;    // Set signal for terminating level loop
                        *agame_state = WORM_SYSWORM_ERROR; // Propagate error situation
                        break; // Terminates loop forall sysworms
                    }
                } else {
                    // Clean the tail of that sysworm
                    cleanWormTail(&theboard, &sysworms[w]);
                    // Now move the worm for one step
                    // Since computeDirection succeeded nothing bad can happen inside moveWorm
                    moveWorm(&theboard, &sysworms[w], &worm_state);
                    // Show the sysworm at its new position
                    showWorm(&theboard, &sysworms[w]);
                }
            }
            // Propagate state; by construction we should always have here
            //  worm_state == WORM_GAME_ONGOING
            *agame_state = worm_state;
        }
        // END process sysworm(s)
