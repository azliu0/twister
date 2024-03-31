/**
 *    file: mt19937.h
 *    author: azliu0
 *    date: 3/30/2024
 *    function prototypes for mt19937.c
 **/

#ifndef MT19937_H
#define MT19937_H

#include <stdint.h>

// state vector size
#define n 624

typedef struct
{
    uint32_t mt[n];
    int index;
} mt19937;

/**
 * seeds generator with given seed
 * throws if seed is 0
 */
void seed_rand(mt19937 *rand, uint32_t seed);

/**
 * generates a random number
 * assumes generator is seeded
 */
uint32_t gen_rand(mt19937 *rand);

/**
 * generates a random number without updating the internal state of the generator
 * assumes generator is seeded
 */
uint32_t gen_rand_no_state_update(mt19937 *rand);

/**
 * updates internal state of generator given list of outputs, and predicts next number
 * throws if nums does not contain exactly n numbers
 */
uint32_t predict(mt19937 *rand, uint32_t *nums);

#endif // MT19937_H