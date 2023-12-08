// random_generator.c

#include <stdlib.h>
#include <time.h>

// Function definition
long long generateRandom11DigitNumber() {
    // Seed the random number generator with the current time
    srand(time(NULL));

    // Generate a random 11-digit number
    return rand() % 90000000000LL + 10000000000LL;
}
