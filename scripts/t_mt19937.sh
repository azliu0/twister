#!/bin/bash

# Compile and run correctness tests for mt19937.c

g++ -std=c++17 -isystem /usr/local/include/gtest t_mt19937.cpp mt19937.c /usr/local/lib/libgtest.a -o t_mt19937
./t_mt19937

rm t_mt19937