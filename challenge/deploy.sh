#!/bin/bash

make clean
make

sudo docker kill mersenne
sudo docker rm mersenne

sudo docker build -t mersenne .
sudo docker run -d -p 4242:4242 --name mersenne mersenne

sudo rm /var/www/html/mersenne
sudo touch /var/www/html/mersenne
sudo cp mersenne_no_link /var/www/html/mersenne
sudo service apache2 restart
