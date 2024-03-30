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
COPY twister/ /root/mersenne/twister/

WORKDIR /root/mersenne/

# create flag
RUN touch flag.txt
RUN echo "hack{vishy_lol}" >> flag.txt

# set permissions 
RUN chmod 600 flag.txt
RUN chmod 4655 mersenne.c

# build executable
RUN make

# expose .so to the executable
ENV LD_LIBRARY_PATH=/root/mersenne:$LD_LIBRARY_PATH

# expose port to ec2 box
EXPOSE 4242 

# expose port to the internet
CMD tcpserver -v -RHl0 0 4242 ./mersenne_no_link
