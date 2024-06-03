#ifndef _RANDOM_H
#define _RANDOM_H

#include <cstdlib>
#include <ctime>

/**
 * The Random class provides methods to initialize the random number generator
 * and generate random integers.
 */
class Random {
    public:
        /**
         * Initializes the random number generator.
         */
        static void randomize() {
            srand(time(NULL));
        }

        /**
         * Returns a random integer between 0 and (bound - 1).
         * 
         * @param bound The upper bound for the random number (exclusive)
         * @return A random integer between 0 and (bound - 1)
         */
        static long random(long bound) {
            return rand() % bound;
        }
};

#endif