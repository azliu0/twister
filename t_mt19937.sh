#!/bin/bash

# Compile and run correctness tests for mt19937.c

g++ t_mt19937.cpp mt19937.c -o t_mt19937
./t_mt19937

rm t_mt19937