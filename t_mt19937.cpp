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

class MT19937Test : public ::testing::Test
{
protected:
    std::random_device rd;
    std::mt19937 mt_base;
    std::unique_ptr<mt19937> mt_impl;

    MT19937Test()
        : mt_impl(std::make_unique<mt19937>())
    {
    }

    void seed_mts()
    {
        uint32_t seed = this->rd();
        this->mt_base.seed(seed);
        seed_rand(mt_impl.get(), seed);
    }
};

TEST_F(MT19937Test, Generator)
{
    for (int _ = 0; _ < SEEDS; _++)
    {
        seed_mts();

        for (int i = 0; i < SEQ_LEN; i++)
        {
            EXPECT_EQ(mt_base(), gen_rand(mt_impl.get()));
        }
    }
}

TEST_F(MT19937Test, PredictorSingle)
{
    for (int _ = 0; _ < SEEDS; _++)
    {
        seed_mts();
        uint32_t state[n];

        for (int i = 0; i < n; i++)
        {
            state[i] = mt_base();
        }

        EXPECT_EQ(mt_base(), predict(mt_impl.get(), state));
    }
}

TEST_F(MT19937Test, PredictorMany)
{
    for (int _ = 0; _ < SEEDS; _++)
    {
        seed_mts();
        uint32_t state[n];

        for (int i = 0; i < n; i++)
        {
            state[i] = mt_base();
        }

        EXPECT_EQ(mt_base(), predict(mt_impl.get(), state));

        for (int i = 0; i < SEQ_LEN; i++)
        {
            EXPECT_EQ(mt_base(), gen_rand(mt_impl.get()));
        }
    }
}

int main()
{
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
