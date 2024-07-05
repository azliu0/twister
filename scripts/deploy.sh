#!/bin/bash

make clean
make

sudo docker kill twister
sudo docker rm twister

sudo docker build -t twister .
sudo docker run -d -p 4242:4242 --name twister twister

# TODO: fix 
sudo rm /var/www/html/mersenne
sudo touch /var/www/html/mersenne
sudo cp challenge/mersenne /var/www/html/mersenne
sudo service apache2 restart
