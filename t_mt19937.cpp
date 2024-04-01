/**
 *    file: t_mt19937.c
 *    author: azliu0
 *    date: 3/30/2024
 *    correctness tests for mt19937.c
 **/

#include <gtest/gtest.h>
#include <random>
#include "mt19937.h"

#define SEEDS 100
#define SEQ_LEN 1000

class mt19937Test : public ::testing::Test
{
protected:
    std::random_device rd;
    std::mt19937 mt_base;
    mt19937 mt_impl;
};

TEST_F(mt19937Test, generator)
{
    for (int _ = 0; _ < SEEDS; _++)
    {
        uint32_t seed = rd();
        mt_base.seed(seed);
        seed_rand(&mt_impl, seed);

        for (int i = 0; i < SEQ_LEN; i++)
        {
            EXPECT_EQ(mt_base(), gen_rand(&mt_impl));
        }
    }
}

int main()
{
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}