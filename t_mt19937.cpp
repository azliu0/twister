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

// generator implementation is consistent with STL
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

// predictor predicts a single value correctly
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

// predictor predicts many values correctly
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

// predictor updates state correctly
TEST_F(MT19937Test, PredictorSelfConsistent)
{
    for (int _ = 0; _ < SEEDS; _++)
    {
        seed_mts();

        std::unique_ptr<mt19937> mt_impl2 = std::make_unique<mt19937>();
        uint32_t state[n];

        for (int i = 0; i < n; i++)
        {
            state[i] = gen_rand(mt_impl.get());
        }

        EXPECT_EQ(gen_rand(mt_impl.get()), predict(mt_impl2.get(), state));

        for (int i = 0; i < n; i++)
        {
            EXPECT_EQ(mt_impl.get()->mt[i], mt_impl2.get()->mt[i]);
        }

        EXPECT_EQ(mt_impl.get()->index, mt_impl2.get()->index);
    }
}

int main()
{
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

// [==========] Running 4 tests from 1 test suite.
// [----------] Global test environment set-up.
// [----------] 4 tests from MT19937Test
// [ RUN      ] MT19937Test.Generator
// [       OK ] MT19937Test.Generator (16 ms)
// [ RUN      ] MT19937Test.PredictorSingle
// [       OK ] MT19937Test.PredictorSingle (9 ms)
// [ RUN      ] MT19937Test.PredictorMany
// [       OK ] MT19937Test.PredictorMany (22 ms)
// [ RUN      ] MT19937Test.PredictorSelfConsistent
// [       OK ] MT19937Test.PredictorSelfConsistent (17 ms)
// [----------] 4 tests from MT19937Test (66 ms total)
//
// [----------] Global test environment tear-down
// [==========] 4 tests from 1 test suite ran. (66 ms total)
// [  PASSED  ] 4 tests.
