/**
 *    file: t_mt19937.c
 *    author: azliu0
 *    date: 3/30/2024
 *    correctness tests for mt19937.c
 **/

#include <random>
#include "mt19937.h"

int main()
{
    std::random_device rd;

    // generator correctness test
    for (int _ = 0; _ < 5; _++)
    {
        uint32_t seed = rd();

        std::mt19937 mt_base(seed);
        mt19937 mt_impl;
        seed_rand(&mt_impl, seed);

        for (int i = 0; i < 1000; i++)
        {
            if (mt_base() != gen_rand(&mt_impl))
            {
                printf("generator test failed\n");
                return 1;
            }
        }
    }

    // predictor correctness test
    for (int _ = 0; _ < 5; _++)
    {
        uint32_t seed = rd();

        std::mt19937 mt_base(seed);
        uint32_t nums[n];

        for (int i = 0; i < n; i++)
        {
            nums[i] = mt_base();
        }

        mt19937 mt_impl;
        uint32_t predicted = predict(&mt_impl, nums);

        if (mt_base() != predicted)
        {
            printf("predictor test failed\n");
            return 1;
        }

        // for (int i = 0; i < 1000; i++)
        // {
        //     if (mt_base() != gen_rand(&mt_impl))
        //     {
        //         printf("predictor test failed\n");
        //         return 1;
        //     }
        // }
    }

    printf("tests passed\n");
}