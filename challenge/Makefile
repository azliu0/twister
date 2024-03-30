# compile flags
CC=gcc
CFLAGS=-fno-stack-protector -I./twister
LDFLAGS=-L. -lmtwister
LIB_NAME=libmtwister.so

# object files for shared library
LIB_SRC=twister/mtwister.c
LIB_OBJ=$(LIB_SRC:.c=.o)

# program files
PROG_SRC=mersenne.c
PROG_TARGET=mersenne

.PHONY: all clean

all: $(LIB_NAME) $(PROG_TARGET)

# compile twister object file
%.o: %.c
	$(CC) -fPIC -c $< -o $@ $(CFLAGS)

# create shared library from object file
# sudo cp $(LIB_NAME) /usr/local/lib
$(LIB_NAME): $(LIB_OBJ)
	$(CC) -shared -o $@ $^

# compile main program
$(PROG_TARGET): $(PROG_SRC)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(LIB_OBJ) $(LIB_NAME) $(PROG_TARGET)
