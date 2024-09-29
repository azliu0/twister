#!/bin/bash

make clean
make

sudo docker kill twister
sudo docker rm twister

sudo docker build -t twister .
sudo docker run -d -p 4242:4242 --name twister twister

# if building with new source code,
# the web app needs to be manually updated with the new binary
