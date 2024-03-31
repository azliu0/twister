/**
 *    author: azliu0
 *    date: 3/30/2024
 *    implements mt19937 generator and predictor
 **/

#include "mt19937.h"

// word size
#define w 32

// twist parameters
#define m 397
#define r 31
#define a 0x9908B0DF
#define mask_upper(x) (x & ((1 << w) - (1 << r))) // upper w-r bits
#define mask_lower(x) (x & ((1 << r) - 1))        // lower r bits

// tempering parameters
#define u 11
#define d 0xFFFFFFFF
#define s 7
#define b 0x9D2C5680
#define t 15
#define c 0xEFC60000
#define l 18

// seeding parameter
#define f 1812433253

/**
 *    generator functions
 */

void seed_rand(mt19937 *rng, uint32_t seed)
{
    if (seed == 0)
    {
        printf("seed cannot be 0\n");
        exit(1);
    }
    rng->mt[0] = seed;
    for (int i = 1; i < n; i++)
    {
        rng->mt[i] = f * (rng->mt[i - 1] ^ (rng->mt[i - 1] >> (w - 2))) + i;
    }
    rng->index = n;
}

uint32_t temper(uint32_t x)
{
    x = x ^ (x >> u);
    x = x ^ ((x << s) & b);
    x = x ^ ((x << t) & c);
    x = x ^ (x >> l);
    return x;
}

void twist(mt19937 *rng)
{
    for (int i = 0; i < n; i++)
    {
        uint32_t x = mask_upper(rng->mt[i]) + mask_lower(rng->mt[(i + 1) % n]);
        rng->mt[i] = rng->mt[(i + m) % n] ^ (x >> 1);
        if (x & 1)
        {
            rng->mt[i] ^= a;
        }
    }
    rng->index = 0;
}

uint32_t gen_rand(mt19937 *rng)
{
    if (rng->index >= n)
    {
        twist(rng);
        rng->index = 0;
    }

    return temper(rng->mt[rng->index++]);
}

uint32_t gen_rand_no_state_update(mt19937 *rng)
{
    if (rng->index >= n)
    {
        twist(rng);
        rng->index = 0;
    }

    return temper(rng->mt[rng->index]);
}

/**
 *    predictor functions
 */

/**
 * returns x where y = x ^ (x >> shamt)
 * main idea here is to telescope:
 *     (x                     ) ^ (x >>           shamt)
 *   ^ (x >>             shamt) ^ (x >>         2*shamt)
 *   ^ (x >>           2*shamt) ^ (x >>         3*shamt)
 *   ...
 *   ^ (x >> (w/shamt-1)*shamt) ^ (x >> (w/shamt)*shamt) = x.
 */
uint32_t un_xor_right(uint32_t y, int shamt)
{
    uint32_t x = y;
    for (int i = 1; i < w / shamt; i++)
    {
        x ^= (y >> (i * shamt));
    }
    return x;
}

/**
 * returns x where y = x ^ ((x << shamt) & mask)
 * main idea here is to directly manipulate the bits.
 *
 * visual representaiton of y:
 *
 * --|--shamt--|--shamt--|--...--|--shamt--|--shamt--|--shamt--|
 * xor
 * --|--sha&m--|--sha&m--|--...--|--sha&m--|--sha&m--|
 *
 *                         (..3)     (2)       (1)       (0)
 *
 * if we can xor away each of the bottom segments, we can recover x.
 * let seg be a mask on segment (0). then,
 *
 *     ((y & seg) << shamt)&mask
 *
 * cancels segment (1), leaving us with
 *
 * --|--shamt--|--shamt--|--...--|--shamt--|--shamt--|--shamt--|
 * xor
 * --|--sha&m--|--sha&m--|--...--|--sha&m--|
 *
 *                         (..3)     (2)       (1)       (0)
 *
 * then mask on segment (1) to cancel segment (2), and so on.
 */
uint32_t un_xor_left_mask(uint32_t y, uint32_t mask, int shamt)
{
    uint32_t seg = (1 << shamt) - 1;
    for (int i = 0; i < w / shamt; i++)
    {
        y ^= (((y & seg) << shamt) & mask);
        seg = seg << shamt;
    }
    return y;
}

uint32_t un_temper(uint32_t y)
{
    y = un_xor_right(y, l);
    y = un_xor_left_mask(y, c, t);
    y = un_xor_left_mask(y, b, s);
    y = un_xor_right(y, u);
    return y;
}

uint32_t predict(mt19937 *rng, uint32_t *nums)
{
    if (sizeof(nums) / sizeof(uint32_t) != n)
    {
        printf("nums must contain exactly %d numbers\n", n);
        exit(1);
    }

    for (int i = 0; i < n; i++)
    {
        rng->mt[i] = un_temper(nums[i]);
    }

    rng->index = n;
    return gen_rand(rng);
}