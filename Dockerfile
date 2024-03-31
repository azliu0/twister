FROM ubuntu:22.04

# install deps
RUN apt-get update && \
    apt-get install -y \
    gcc \
    make \
		ucspi-tcp

# copy files
COPY Makefile /root/mersenne/
COPY mersenne.c /root/mersenne/
COPY mt19937.c /root/mersenne/
COPY mt19937.h /root/mersenne/

WORKDIR /root/mersenne/

# create flag
RUN touch flag.txt
RUN echo "hack{vishy_lol}" >> flag.txt

# set permissions 
RUN chmod 600 flag.txt
RUN chmod 4655 mersenne.c

# build executable
RUN mkdir challenge
RUN make
RUN mv challenge/mersenne .

# expose port to box
EXPOSE 4242 

# expose port to the internet
CMD tcpserver -v -RHl0 0 4242 ./mersenne
