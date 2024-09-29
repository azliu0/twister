FROM ubuntu:22.04

# install deps
RUN apt-get update && apt-get install -y gcc make ucspi-tcp
RUN useradd -m twister

# copy files
COPY Makefile /home/twister/twister/
COPY twister.c /home/twister/twister/
COPY mt19937.c /home/twister/twister/
COPY mt19937.h /home/twister/twister/

WORKDIR /home/twister/twister/

# build executable and set permissions
RUN mkdir binaries
RUN make
RUN mv binaries/twister .
RUN chown -R root:twister .
RUN find . -type f -exec chmod 440 {} +
RUN find . -type d -exec chmod 550 {} +
RUN chmod 4655 twister

# create flag
RUN touch flag.txt && \
    echo "hack{rKyPyZUS0M8_54e8OnvLWuM_yEAAfKjfR9Q}" >> flag.txt && \
    chown root:twister flag.txt && \
    chmod 440 flag.txt

# expose port to box
EXPOSE 4242 

# expose port to the internet
USER twister
CMD tcpserver -v -RHl0 0 4242 ./twister
