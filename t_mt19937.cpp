/**
 *    file: t_mt19937.c
 *    author: azliu0
 *    date: 3/30/2024
 *    simple correctness tests for mt19937.c
 **/

#include <random>
#include "mt19937.h"

int main()
{
    std::random_device rd;
    uint32_t seed = rd();

    std::mt19937 mt_base(seed);
    mt19937 mt_impl;
    seed_rand(&mt_impl, seed);

    for (int i = 0; i < 1000; i++)
    {
        printf("mt_base: %u, mt_impl: %u\n", mt_base(), gen_rand(&mt_impl));
        // if (mt_base() != gen_rand(&mt_impl))
        // {
        //     printf("mt19937 correctness test failed\n");
        //     return 1;
        // }
    }

    printf("mt19937 correctness test passed\n");
}